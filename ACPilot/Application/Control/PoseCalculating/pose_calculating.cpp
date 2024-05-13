//
// Created by zhaohe on 2024/4/18.
//

#include "pose_calculating.h"
#include "Nvs/nvs_driver.h"
#include "AHRS/Mahony/mahony.h"
#include "Notify/notify.h"
#include "HardTimer/hard_timer_driver.h"
#include "Board/board_esp32_mini.h"
#include "Filter/IIR/lpf.h"

static void suspend(void *param)
{
    PoseCalculating::run = false;
}

static void resume(void *param)
{
    PoseCalculating::run = true;
}

volatile bool PoseCalculating::run = true;
Mailbox<PoseData> *PoseCalculating::mailbox = nullptr;
Accelerometer *PoseCalculating::acc = nullptr;
Gyroscope *PoseCalculating::gyro = nullptr;
Attitude *PoseCalculating::attitude = nullptr;
uint64_t PoseCalculating::last_time = 0;

AC_RET PoseCalculating::init()
{
    DeviceCaliData cali;
    Nvs *fd = nullptr;

    mailbox = Mailbox<PoseData>::find("attitude");

    if (mailbox == nullptr)
    {
        BASE_ERROR("attitude mailbox not found");
        return AC_ERROR;
    }

    acc = VirtualDevice::find<Accelerometer>("acc", ACCELEROMETER_DEV);
    gyro = VirtualDevice::find<Gyroscope>("gyro", GYROSCOPE_DEV);

    if (acc == nullptr || gyro == nullptr)
    {
        BASE_ERROR("acc or gyro is nullptr");
        return AC_ERROR;
    }

    fd = Nvs::open("calibration");
    if (fd == nullptr)
    {
        BASE_ERROR("open calibration data failed");
        return AC_ERROR;
    }

    fd->read("gyro", &cali, sizeof(DeviceCaliData));
    gyro->setCali(cali);

    fd->read("acc", &cali, sizeof(DeviceCaliData));
    acc->setCali(cali);

    Nvs::close(fd);
    Filter *gx_filter = new SecondOrderLPF();
    gx_filter->init(1000.f, 100.f);
    Filter *gy_filter = new SecondOrderLPF();
    gy_filter->init(1000.f, 100.f);
    Filter *gz_filter = new SecondOrderLPF();
    gz_filter->init(1000.f, 100.f);
    Filter *ax_filter = new SecondOrderLPF();
    ax_filter->init(1000.f, 100.f);
    Filter *ay_filter = new SecondOrderLPF();
    ay_filter->init(1000.f, 100.f);
    Filter *az_filter = new SecondOrderLPF();
    az_filter->init(1000.f, 100.f);

    attitude = new Attitude(new Mahony(), gx_filter, gy_filter, gz_filter, ax_filter, ay_filter, az_filter);

    Notify::sub(ENTER_CALI_EVENT, suspend);

    Notify::sub(LEAVE_CALI_EVENT, resume);

    return AC_OK;
}

AC_RET PoseCalculating::step(AircraftState &state)
{
    if (!run)
    {
        attitude->reset();
        tickSleep(1);
        return AC_OK;
    }

    MemsData mems{MEMS_6_AXIS};

    acc->read(mems.acc);
    gyro->read(mems.gyro);

    uint64_t current_time = HardwareTimer::getCurrentTime();
    uint32_t delta_time = current_time - last_time;

    if (last_time == 0)
    {
        last_time = current_time;
        return AC_OK;
    } else
    {
        attitude->update(mems, US_TO_S(delta_time));
    }
    last_time = current_time;

    state.pose = attitude->pose;
    state.earth_acc = attitude->earth_acc;

    return AC_OK;
}
