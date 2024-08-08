//
// Created by zhaohe on 2024/4/20.
//

#include "sensor_calibrate.h"
#include "Algorithm/BiasDetect/bias_detect.h"
#include "Algorithm/EllipsoidFit/ellipsoid_fit.h"
#include "Algorithm/RotaryTrans/rotary_trans.h"
#include "Debug/default_debug.h"
#include "os.h"
#include "Light/light_server.h"
#include "Device/Virtual/Accelerometer/accelerometer.h"
#include "Device/Virtual/Gyroscope/gyroscope.h"
#include "Notify/notify.h"
#include "Command/Calibrate/calibrate_command.h"
#include "Nvs/nvs_driver.h"

static Osal::AcThread *task = nullptr;
static Utils::Mailbox<Service::LightMessage> *light_mailbox = nullptr;
static Utils::Mailbox<Component::CaliMessage> *cali_mailbox = nullptr;
static Utils::Mailbox<ComMessage> *send_mailbox = nullptr;
static Utils::MemoryPool *memory_pool = nullptr;
static Framework::Gyroscope *gyro = nullptr;
static Framework::Accelerometer *acc = nullptr;

#define CALI_REPLY_LEN 128

static void reply(const char *buf, uint8_t port)
{
    ComMessage msg;
    msg.port = port;
    msg.pool = memory_pool;
    msg.buf = memory_pool->alloc();
    msg.len = strlen(buf) + 1;
    memcpy(msg.buf, buf, msg.len);
    send_mailbox->push(&msg);
}

#define CALI_GYRO_SAMPLING_NUM 20

void caliGyro(uint8_t port)
{
    Service::LightMessage light_msg;
    Component::BiasDetect bias_detect;
    Component::CaliMessage cmd;
    Common::Vec3 *data = new Common::Vec3[CALI_GYRO_SAMPLING_NUM];

    reply("sampling start\n", port);

    light_msg.id = 0x01;
    light_msg.mode = Service::LIGHT_FAST_FLASHING;
    light_mailbox->push(&light_msg);
    while (1)
    {
        cali_mailbox->pop(&cmd);
        if (cmd.cmd == Component::CALI_CMD_STOP)
        {
            goto exit;
        }
        if (cmd.cmd == Component::CALI_CMD_SAMPLING)
        {
            break;
        }
    }
    light_msg.id = 0x01;
    light_msg.mode = Service::LIGHT_KEEP_ON;
    light_mailbox->push(&light_msg);

    for (int i = 0; i < CALI_GYRO_SAMPLING_NUM; i++)
    {
        gyro->read(data[i]);
        tickSleep(100);
    }
    reply("sampling finish\n", port);
    gyro->cali(data, CALI_GYRO_SAMPLING_NUM, &bias_detect);
    exit:
    delete[] data;
}

#define CALI_ACC_ELLIPSOID_SAMPLING_NUM 150

void caliAccEllipsoid(uint8_t port)
{
    Service::LightMessage light_msg;
    Component::EllipsoidFit ellipsoid_fit{1000.0F};
    Component::CaliMessage cmd;
    Common::Vec3 *data = new Common::Vec3[CALI_ACC_ELLIPSOID_SAMPLING_NUM];

    reply("sampling start\n", port);
    for (int i = 0; i < 6; i++)
    {
        light_msg.id = 0x01;
        light_msg.mode = Service::LIGHT_FAST_FLASHING;
        light_mailbox->push(&light_msg);
        while (1)
        {
            cali_mailbox->pop(&cmd);
            if (cmd.cmd == Component::CALI_CMD_STOP)
            {
                goto exit;
            }
            if (cmd.cmd == Component::CALI_CMD_SAMPLING)
            {
                break;
            }
        }
        light_msg.id = 0x01;
        light_msg.mode = Service::LIGHT_KEEP_ON;
        light_mailbox->push(&light_msg);
        for (int j = 0; j < CALI_ACC_ELLIPSOID_SAMPLING_NUM / 6; j++)
        {
            acc->read(data[CALI_ACC_ELLIPSOID_SAMPLING_NUM / 6 * i + j]);

            tickSleep(100);
        }
    }
    reply("sampling finish\n", port);
    acc->cali(data, CALI_ACC_ELLIPSOID_SAMPLING_NUM, &ellipsoid_fit);
    exit:
    delete[] data;
}

#define CALI_ACC_ROTARY_SAMPLING_NUM 2

void caliRotary(uint8_t port)
{
    Service::LightMessage light_msg;
    Component::RotaryTrans rotary_trans;
    Component::CaliMessage cmd;
    Common::Vec3 temp;
    Common::Vec3 *data = new Common::Vec3[CALI_ACC_ROTARY_SAMPLING_NUM];

    for (int i = 0; i < CALI_ACC_ROTARY_SAMPLING_NUM; i++)
    {
        light_msg.id = 0x01;
        light_msg.mode = Service::LIGHT_FAST_FLASHING;
        light_mailbox->push(&light_msg);
        while (1)
        {
            cali_mailbox->pop(&cmd);
            if (cmd.cmd == Component::CALI_CMD_STOP)
            {
                goto exit;
            }
            if (cmd.cmd == Component::CALI_CMD_SAMPLING)
            {
                break;
            }
        }
        light_msg.id = 0x01;
        light_msg.mode = Service::LIGHT_KEEP_ON;
        light_mailbox->push(&light_msg);
        for (int j = 0; j < 10; j++)
        {
            acc->read(temp);
            data[i].x += temp.x;
            data[i].y += temp.y;
            data[i].z += temp.z;
            tickSleep(100);
        }
        data[i].x /= 10;
        data[i].y /= 10;
        data[i].z /= 10;
    }
    reply("sampling finish\n", port);
    acc->cali(data, CALI_ACC_ROTARY_SAMPLING_NUM, &rotary_trans);
    gyro->copyRot(acc->getCali());
    exit:
    delete[] data;
}

void sensorCalibrateTask(void *param)
{
    Component::CaliMessage cmd;
    Service::LightMessage light_msg;

    cali_mailbox->pop(&cmd);
    if (cmd.cmd == Component::CALI_CMD_SIMPLE)
    {
        gyro->clearCali();
        reply("input '>cali sampling' to continue\n", cmd.port);
        caliGyro(cmd.port);

    } else if (cmd.cmd == Component::CALI_CMD_ADVANCED)
    {
        Driver::Nvs *fd = Driver::Nvs::open("calibration");
        if (fd == nullptr)
        {
            BASE_ERROR("open calibration data failed");
            Utils::Notify::notify(CALI_FINISH_EVENT);
            Osal::AcThread::killSelf();
        }
        gyro->clearCali();
        reply("gyro cali. input '>cali sampling' to continue\n", cmd.port);
        caliGyro(cmd.port);
        acc->clearCali();
        reply("ellipsoid cali. input '>cali sampling' to continue\n", cmd.port);
        caliAccEllipsoid(cmd.port);
        reply("rotary cali. input '>cali sampling' to continue\n", cmd.port);
        caliRotary(cmd.port);
        reply("rotary cali. input '>cali save' to save\n", cmd.port);
        light_msg.id = 0x01;
        light_msg.mode = Service::LIGHT_FAST_FLASHING;
        light_mailbox->push(&light_msg);
        cali_mailbox->pop(&cmd);
        if (cmd.cmd == Component::CALI_CMD_SAVE)
        {
            fd->write("gyro", gyro->getCali(), sizeof(Framework::DeviceCaliData));
            fd->write("acc", acc->getCali(), sizeof(Framework::DeviceCaliData));
            fd->save();
            reply("calibrate data save success\n", cmd.port);
        } else
        {
            reply("calibrate data not save\n", cmd.port);
        }
        Driver::Nvs::close(fd);
    }
    light_msg.id = 0x01;
    light_msg.mode = Service::LIGHT_BREATHE;
    light_mailbox->push(&light_msg);
    Utils::Notify::notify(CALI_FINISH_EVENT);
    Osal::AcThread::killSelf();
}

void startPoseCalculatingCb(void *param)
{
    if (task == nullptr)
    {
        return;
    }
    task->run(sensorCalibrateTask, nullptr);
}

void registerSensorCalibrateTask()
{
    light_mailbox = Utils::Mailbox<Service::LightMessage>::find("light");
    cali_mailbox = Utils::Mailbox<Component::CaliMessage>::find("cali");
    send_mailbox = Utils::Mailbox<ComMessage>::find("send");
    memory_pool = Utils::MemoryPool::getGeneral(CALI_REPLY_LEN);
    gyro = Framework::VirtualDevice::find<Framework::Gyroscope>("gyro", Framework::GYROSCOPE_DEV);
    acc = Framework::VirtualDevice::find<Framework::Accelerometer>("acc", Framework::ACCELEROMETER_DEV);
    if (light_mailbox == nullptr || cali_mailbox == nullptr || send_mailbox == nullptr || memory_pool == nullptr ||
        gyro == nullptr || acc == nullptr)
    {
        BASE_ERROR("object is null");
        return;
    }
    task = new Osal::AcThread("cali", CALI_TASK_STACK_SIZE, CALI_TASK_PRIO, CALI_TASK_CORE);
    Utils::Notify::sub(ENTER_CALI_EVENT, startPoseCalculatingCb);
}