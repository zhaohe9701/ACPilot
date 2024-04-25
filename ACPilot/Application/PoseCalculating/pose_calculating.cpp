//
// Created by zhaohe on 2024/4/18.
//

#include "pose_calculating.h"
#include "Thread/ac_thread.h"
#include "Notify/notify.h"
#include "Device/Virtual/Accelerometer/accelerometer.h"
#include "Device/Virtual/Gyroscope/gyroscope.h"
#include "AHRS/ahrs.h"
#include "AHRS/Mahony/mahony.h"
#include "AHRS/attitude.h"
#include "HardTimer/hard_timer_driver.h"
#include "Nvs/nvs_driver.h"

static AcThread *task = nullptr;
static HardwareTimer *timer = nullptr;
static AcBinSemaphore *sem = nullptr;
static bool run = true;

void suspend(void *param)
{
    run = false;
    timer->stop();
}

void resume(void *param)
{
    timer->start(1000, TIMER_MODE_PERIODIC);
    run = true;
}

void timerCb(void *param)
{
    sem->give();
}

void poseCalculatingTask(void *param)
{
    Accelerometer *acc = static_cast<Accelerometer *>(VirtualDevice::find("acc", ACCELEROMETER_DEV));
    Gyroscope *gyro = static_cast<Gyroscope *>(VirtualDevice::find("gyro", GYROSCOPE_DEV));

    if (acc == nullptr || gyro == nullptr)
    {
        BASE_ERROR("acc or gyro is nullptr");
        AcThread::killSelf();
    }
    Nvs *fd = Nvs::open("calibration");
    if (fd == nullptr)
    {
        BASE_ERROR("open calibration data failed");
        AcThread::killSelf();
    }

    DeviceCaliData cali;

    fd->read("gyro", &cali, sizeof(DeviceCaliData));
    gyro->setCali(cali);

    fd->read("acc", &cali, sizeof(DeviceCaliData));
    acc->setCali(cali);

    Nvs::close(fd);

    Attitude attitude{new Mahony()};
    int i = 0;
    for (;;)
    {
        if (!run)
        {
            attitude.reset();
            tickSleep(1);
            continue;
        }
        sem->get();
        MemsData mems{MEMS_6_AXIS};
        acc->read(mems.acc);
        gyro->read(mems.gyro);
        attitude.update(mems, 0.001);
        if (i == 100)
        {
//            if((mems.gyro.x * mems.gyro.x + mems.gyro.y * mems.gyro.y + mems.gyro.z * mems.gyro.z < 2))
//            {
//                printf("%f %f %f\n", mems.acc.x, mems.acc.y, mems.acc.z);
//            }
//            printf("%f %f %f\n", mems.acc.x, mems.acc.y, mems.acc.z);
//            printf("%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\n", mems.gyro.x, mems.gyro.y, mems.gyro.z, mems.acc.x,
//                   mems.acc.y, mems.acc.z);
//            printf("euler: %f, %f, %f\n", attitude.euler.roll, attitude.euler.pitch, attitude.euler.yaw);
            i = 0;
        } else
        {
            i++;
        }

    }
}


void registerPoseCalculatingTask()
{
    task = new AcThread("pose", 4000, 24);
    timer = new HardwareTimer("pose", timerCb, nullptr);
    sem = new AcBinSemaphore();

    Notify::sub(ENTER_CALIBRATE_EVENT, suspend);

    Notify::sub(LEAVE_CALIBRATE_EVENT, resume);

    timer->init();

    timer->start(1000, TIMER_MODE_PERIODIC);

    task->run(poseCalculatingTask, nullptr);
}