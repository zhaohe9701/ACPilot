//
// Created by zhaohe on 2024/5/9.
//

#include "altitude_control.h"
#include "config.h"
#include "Device/Virtual/Altimeter/altimeter.h"
#include "HardTimer/hard_timer_driver.h"

Mailbox<AltitudeData> *AltitudeControl::_baro_mailbox = nullptr;
AcThread *AltitudeControl::_baro_task = nullptr;
AltitudeEstimate *AltitudeControl::_estimator = nullptr;
Vec3 AltitudeControl::_acc_bias{};
uint64_t AltitudeControl::last_time = 0;
float AltitudeControl::_height_bias = 0.f;

AC_RET AltitudeControl::init()
{
    _baro_mailbox = new Mailbox<AltitudeData>("altitude", 1);
    _estimator = new AltitudeEstimate();
    _estimator->init(0.03f, 0.2f, 0.0001, 0.00001, 0.0001);
    _baro_task = new AcThread("altitude", BARO_TASK_STACK_SIZE, BARO_TASK_PRIO, BARO_TASK_CORE);
    _baro_task->run(_altitudeSensorUpdate, nullptr);
    tickSleep(100);
    AltitudeData altitude;
    _baro_mailbox->copy(&altitude, AC_IMMEDIATELY);
    _height_bias = altitude.x;
    return AC_OK;
}

void AltitudeControl::setAccBias(Vec3 &acc_bias)
{
    _acc_bias = acc_bias;
}

AC_RET AltitudeControl::step(ExpectState &expect_state, AircraftState &state)
{
    float x[3] = {0.f};
    AltitudeData altitude;
    uint64_t current_time = 0;
    uint32_t delta_time = 0;
    RETURN_CHECK(_baro_mailbox->copy(&altitude, AC_IMMEDIATELY));
    current_time = HardwareTimer::getCurrentTime();
    if (last_time == 0)
    {
        last_time = current_time;
        return AC_OK;
    }
    delta_time = current_time - last_time;
    last_time = current_time;

    _estimator->calculate(altitude.x - _height_bias, (state.earth_acc.z - _acc_bias.z) / 100.f, x, US_TO_S(delta_time));
//    printf("%f %f %f %f %f\n", altitude.x - _height_bias
//                             , (state.earth_acc.z - _acc_bias.z) / 100.f
//                             , US_TO_S(delta_time)
//                             , x[0]
//                             , x[1]);
    state.earth_pos.z = x[0];
    state.earth_vel.z = x[1];

    return AC_OK;
    error:
    return AC_ERROR;
}

void AltitudeControl::reset()
{

}

void AltitudeControl::clear()
{

}

void AltitudeControl::_altitudeSensorUpdate(void *param)
{
    Altimeter *altimeter = static_cast<Altimeter *>(VirtualDevice::find("altimeter", ALTIMETER_DEV));
    AltitudeData altitude;
    if (altimeter == nullptr)
    {
        BASE_ERROR("altimeter not found");
        AcThread::killSelf();
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


