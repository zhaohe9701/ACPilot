//
// Created by zhaohe on 2024/5/9.
//

#include "altitude_control.h"
#include "config.h"
#include "Device/Virtual/Altimeter/altimeter.h"
#include "HardTimer/hard_timer_driver.h"
#include "dm_template.h"
#include "DataModule/data_module.h"

Utils::Mailbox<Framework::AltitudeData> *AltitudeControl::_baro_mailbox = nullptr;
Osal::AcThread *AltitudeControl::_baro_task = nullptr;
Component::AltitudeEstimate *AltitudeControl::_estimator = nullptr;
Common::Vec3 AltitudeControl::_acc_bias{};
uint64_t AltitudeControl::last_time = 0;
float AltitudeControl::_height_bias = 0.f;
float AltitudeControl::_aim_height = 0.f;
float AltitudeControl::_base_throttle = 30.f;
bool AltitudeControl::_isTakeOff = false;
Component::Pid *AltitudeControl::_vel_pid = nullptr;
Component::Pid *AltitudeControl::_pos_pid = nullptr;

AC_RET AltitudeControl::init()
{
    _baro_mailbox = new Utils::Mailbox<Framework::AltitudeData>("altitude", 1);
    _estimator = new Component::AltitudeEstimate();
    _estimator->init(0.03f, 0.2f, 0.0001, 0.00001, 0.0001);
    _baro_task = new Osal::AcThread("altitude", BARO_TASK_STACK_SIZE, BARO_TASK_PRIO, BARO_TASK_CORE);
    _baro_task->run(_altitudeSensorUpdate, nullptr);

    _vel_pid = new Component::Pid();
    _pos_pid = new Component::Pid();

    PidConfig config;
    RETURN_CHECK(Framework::DataModule::read("/controller/pos[2]/1", &config, sizeof(PidConfig)));
    _pos_pid->init(config.kp, config.ki, config.kd, config.out_limit, config.int_limit);
    RETURN_CHECK(Framework::DataModule::read("/controller/pos[2]/2", &config, sizeof(PidConfig)));
    _vel_pid->init(config.kp, config.ki, config.kd, config.out_limit, config.int_limit);

    return AC_OK;
    error:
    BASE_ERROR("pos control init failed");
    return AC_ERROR;
}

void AltitudeControl::setAccBias(Common::Vec3 &acc_bias)
{
    _acc_bias = acc_bias;
}

AC_RET AltitudeControl::step(Component::ExpectState &expect_state, AircraftState &state)
{
    float x[3] = {0.f};
    float aim_vel = 0.f;
    Framework::AltitudeData altitude;
    uint64_t current_time = 0;
    uint32_t delta_time = 0;
    /* 获取气压数据 */
    RETURN_CHECK(_baro_mailbox->copy(&altitude, AC_IMMEDIATELY));

    current_time = Driver::HardwareTimer::getCurrentTime();
    if (last_time == 0)
    {
        last_time = current_time;
        return AC_OK;
    }
    delta_time = current_time - last_time;
    last_time = current_time;

    /* 一键起飞 */
    if (!_isTakeOff)
    {
        /* 油门超过中位即起飞 */
        if (expect_state.height_rate < 0)
        {
            /* 未超过中位,刷新起始高度 */
            _height_bias = altitude.x;
            expect_state.height_rate = 0.f;
            expect_state.throttle = 0.f;
            _aim_height = 0.0f;
        } else
        {
            /* 超过中位,设置起飞高度 */
            _aim_height = 0.8f;
            _isTakeOff = true;
        }
    }

    /* 设置目标高度, 为稳定定高设置一段死区 */
    if (expect_state.height_rate < -0.1f)
    {
        _aim_height += (expect_state.height_rate + 0.1f) * US_TO_S(delta_time);
    } else if (expect_state.height_rate > 0.1f)
    {
        _aim_height += (expect_state.height_rate - 0.1f) * US_TO_S(delta_time);
    }
//    printf("aim height:%f\n", _aim_height);
    /* 状态估计 */
    _estimator->calculate(altitude.x - _height_bias, (state.earth_acc.z - _acc_bias.z) / 100.f, x, US_TO_S(delta_time));
//    printf("%f %f %f %f %f\n", altitude.x - _height_bias
//                             , (state.earth_acc.z - _acc_bias.z) / 100.f
//                             , US_TO_S(delta_time)
//                             , x[0]
//                             , x[1]);
    state.earth_pos.z = x[0];
    state.earth_vel.z = x[1];
    if (_isTakeOff)
    {
        /* 速度环 */
        aim_vel = _pos_pid->output(_aim_height - state.earth_pos.z, US_TO_S(delta_time));
        /* 位置环 */
        expect_state.throttle = _vel_pid->output(aim_vel - state.earth_vel.z, US_TO_S(delta_time));
        expect_state.throttle += _base_throttle;
    }
    return AC_OK;
    error:
    return AC_ERROR;
}

void AltitudeControl::reset()
{
    PidConfig config;
    RETURN_CHECK(Framework::DataModule::read("/controller/pos[2]/1", &config, sizeof(PidConfig)));
    _pos_pid->init(config.kp, config.ki, config.kd, config.out_limit, config.int_limit);
    RETURN_CHECK(Framework::DataModule::read("/controller/pos[2]/2", &config, sizeof(PidConfig)));
    _vel_pid->init(config.kp, config.ki, config.kd, config.out_limit, config.int_limit);
    return;
    error:
    BASE_ERROR("pos control reset failed");
}

void AltitudeControl::clear()
{
    if (nullptr == _vel_pid || nullptr == _pos_pid)
    {
        return;
    }
    _vel_pid->clear();
    _pos_pid->clear();
    Framework::AltitudeData altitude;
    _baro_mailbox->copy(&altitude, AC_IMMEDIATELY);
    _height_bias = altitude.x;

    _aim_height = 0.f;

    _isTakeOff = false;
}

void AltitudeControl::_altitudeSensorUpdate(void *param)
{
    Framework::Altimeter *altimeter = Framework::VirtualDevice::find<Framework::Altimeter>("altimeter",
                                                                                           Framework::ALTIMETER_DEV);
    Framework::AltitudeData altitude;
    if (altimeter == nullptr)
    {
        BASE_ERROR("altimeter not found");
        Osal::AcThread::killSelf();
        return;
    }

    Tick last_tick;
    Tick delay = msToTick(30);
    last_tick = getTick();
    for (;;)
    {
        tickSleepUntil(&last_tick, delay);
        altimeter->read(altitude);
        _baro_mailbox->update(&altitude);
    }
}




