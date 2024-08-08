//
// Created by zhaohe on 2024/4/28.
//

#include "control.h"
#include "Remote/remote.h"
#include "Thread/thread.h"
#include "Notify/notify.h"
#include "Motor/motor.h"
#include "Motor/PwmMotor/pwm_motor.h"
#include "Board/board_esp32_mini.h"
#include "Control/PoseControl/pose_control.h"
#include "Control/PoseCalculating/pose_calculating.h"
#include "Light/light_server.h"
#include "AircraftState/aircraft_state.h"
#include "Algorithm/Variance/variance.h"
#include "Control/AltitudeControl/altitude_control.h"

#define F1000HZ     1
#define F500HZ      2
#define F250HZ      4
#define F200HZ      5
#define F100HZ      10
#define F50HZ       20

#define _1000HZ     (1000)
#define _500HZ      (500)
#define _250HZ      (250)
#define _200HZ      (200)
#define _100HZ      (100)
#define _50HZ       (50)

#define IS_TIME_TO_GO(n, t) (((n) % (t) == 0)?true:false)
#define TICK_UP(n) ((n)++)

static Osal::AcThread *main_control_task = nullptr;
static Osal::AcThread *pos_control_task = nullptr;

static Utils::Mailbox<ComMessage> *send_mailbox = nullptr;
static Utils::Mailbox<Component::ExpectState> *expect_state_mailbox = nullptr;
static Utils::Mailbox<AircraftState> *actual_state_mailbox = nullptr;

volatile static uint8_t mode = 0;

static uint64_t tick = 0;

static void modeCb(void *param)
{
    Utils::NotifyToken *token = (Utils::NotifyToken *) param;
    switch (token->getEvent())
    {
        case ENTER_MANUAL_EVENT:
            mode = 1;
            break;
        case ENTER_HEIGHT_EVENT:
            mode = 2;
            break;
        default:
            mode = 0;
            break;
    }
}

static void caliGyro()
{

}

static void collectAccBias(Common::Vec3 &acc_bias)
{
    AircraftState state;
    Common::Vec3 *acc = new Common::Vec3[1000];
    Common::Vec3 acc_variance;
    do{
        BASE_INFO("acc bias collect......");
        for (int i = 0; i < 1000; i++)
        {
            Board::imu_interrupt->waitNotify();
            PoseCalculating::step(state);
            acc[i] = state.earth_acc;
        }
        Component::Variance::calculate(acc, 1000, acc_variance);
    } while (acc_variance.x > 3.0 || acc_variance.y > 3.0 || acc_variance.z > 3.0);
    BASE_INFO("acc bias collect finish");
    for (int i = 0; i < 1000; i++)
    {
        acc_bias.x += acc[i].x;
        acc_bias.y += acc[i].y;
        acc_bias.z += acc[i].z;
    }
    acc_bias.x /= 1000;
    acc_bias.y /= 1000;
    acc_bias.z /= 1000;
    delete[] acc;
}

void posControlTask(void *param)
{
    Common::Vec3 acc_bias = *static_cast<Common::Vec3 *>(param);
    Component::ExpectState expect_state;
    AircraftState state;
    Tick last_tick;
    AltitudeControl::setAccBias(acc_bias);
    Tick delay = msToTick(1000 / _100HZ);
    last_tick = getTick();

    for (;;)
    {
        tickSleepUntil(&last_tick, delay);
        if (mode == 2)
        {
            expect_state_mailbox->copy(&expect_state, AC_IMMEDIATELY);
            actual_state_mailbox->copy(&state, AC_IMMEDIATELY);

            AltitudeControl::step(expect_state, state);

            expect_state_mailbox->update(&expect_state);
//            printf("h:%f v:%f\n", state.earth_pos.z, state.earth_vel.z);
        } else
        {
            AltitudeControl::clear();
        }
    }
}

void mainControlTask(void *param)
{
    Component::ExpectState expect_state;
    AircraftState state;
    Common::Vec3 control_output;
    Common::Vec3 acc_bias;
    float motor[4] = {0.0f, 0.0f, 0.0f, 0.0f};
    float expect_yaw = 0.0f;

    expect_state_mailbox->update(&expect_state);
    actual_state_mailbox->update(&state);

    caliGyro();
    collectAccBias(acc_bias);

    pos_control_task->run(posControlTask, &acc_bias);

    Utils::Notify::notify(INIT_FINISH_EVENT);

    for (;;)
    {
        Board::imu_interrupt->waitNotify();

        actual_state_mailbox->copy(&state, AC_IMMEDIATELY);
        PoseCalculating::step(state);
        actual_state_mailbox->update(&state);


        if (IS_TIME_TO_GO(tick, F500HZ))
        {
            expect_state_mailbox->copy(&expect_state, AC_IMMEDIATELY);
            Component::ExpectState new_expect_state;
            Remote::analysis(new_expect_state);
            if (mode == 1)
            {
                expect_state.euler = new_expect_state.euler;
                expect_state.throttle = new_expect_state.throttle;
            } else if (mode == 2)
            {
                expect_state.euler = new_expect_state.euler;
                expect_state.height_rate = new_expect_state.height_rate;
            }
            expect_state_mailbox->update(&expect_state);
        }

        if (IS_TIME_TO_GO(tick, F500HZ))
        {
            /* 计算期望yaw */
            if (mode == 1 || mode == 2)
            {
                expect_state.euler.yaw = expect_yaw + expect_state.euler.yaw / (float)_500HZ;
                if (expect_state.euler.yaw > 180.0f)
                {
                    expect_state.euler.yaw -= 360.0f;
                } else if (expect_state.euler.yaw < -180.0f)
                {
                    expect_state.euler.yaw += 360.0f;
                }
                expect_yaw = expect_state.euler.yaw;
            }
            /* 姿态控制 */
            if (mode == 1 || mode == 2 /*|| mode == 3*/)
            {
                control_output.clear();
                PoseControl::step(expect_state, state.pose, control_output);
            } else
            {
                expect_yaw = state.pose.euler.yaw;
                PoseControl::clear();
            }
        }
        if (IS_TIME_TO_GO(tick, F500HZ))
        {
            if (mode == 1 || mode == 2)
            {
                motor[0] = expect_state.throttle - control_output.y - control_output.x - control_output.z;
                motor[1] = expect_state.throttle - control_output.y + control_output.x + control_output.z;
                motor[2] = expect_state.throttle + control_output.y + control_output.x - control_output.z;
                motor[3] = expect_state.throttle + control_output.y - control_output.x + control_output.z;
                Motor::set(0x00, motor[0]);
                Motor::set(0x01, motor[1]);
                Motor::set(0x02, motor[2]);
                Motor::set(0x03, motor[3]);
            } else
            {
                Motor::set(0x00, 0.f);
                Motor::set(0x01, 0.f);
                Motor::set(0x02, 0.f);
                Motor::set(0x03, 0.f);
            }
        }

//        if (IS_TIME_TO_GO(tick, F50HZ) & (mode == 1 || mode == 2))
//        {
//            ComMessage msg;
//            msg.port = 0x01;
//            msg.pool = MemoryPool::getGeneral(64);
//            msg.buf = static_cast<MemoryPool *>(msg.pool)->alloc();
//            snprintf((char *) msg.buf, 64, "%f %f %f\n", state.earth_acc.x, state.earth_acc.y, state.earth_acc.z);
//            msg.len = strlen((char *) msg.buf);
//            send_mailbox->push(&msg);
//        }
        TICK_UP(tick);
    }
}

void registerControlTask()
{
    main_control_task = new Osal::AcThread("m_control", MAIN_CONTROL_TASK_STACK_SIZE, MAIN_CONTROL_TASK_PRIO, MAIN_CONTROL_TASK_CORE);
    pos_control_task = new Osal::AcThread("pos_control", POS_CONTROL_TASK_STACK_SIZE, POS_CONTROL_TASK_PRIO, POS_CONTROL_TASK_CORE);

    send_mailbox = Utils::Mailbox<ComMessage>::find("send");

    expect_state_mailbox = new Utils::Mailbox<Component::ExpectState>("expect", 1);
    actual_state_mailbox = new Utils::Mailbox<AircraftState>("actual", 1);

    Utils::Notify::sub(ENTER_MANUAL_EVENT, modeCb, nullptr);
    Utils::Notify::sub(ENTER_HEIGHT_EVENT, modeCb, nullptr);
    Utils::Notify::sub(LEAVE_MANUAL_EVENT, modeCb, nullptr);
    Utils::Notify::sub(LEAVE_HEIGHT_EVENT, modeCb, nullptr);

    new Component::PwmMotor(Board::pwm0, 0x00);
    new Component::PwmMotor(Board::pwm1, 0x01);
    new Component::PwmMotor(Board::pwm2, 0x02);
    new Component::PwmMotor(Board::pwm3, 0x03);

    Remote::init();
    PoseControl::init();
    AltitudeControl::init();

    main_control_task->run(mainControlTask, nullptr);
}