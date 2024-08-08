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
Utils::Mailbox<Component::PoseData> *PoseCalculating::mailbox = nullptr;
Framework::Accelerometer *PoseCalculating::acc = nullptr;
Framework::Gyroscope *PoseCalculating::gyro = nullptr;
Component::Attitude *PoseCalculating::attitude = nullptr;
uint64_t PoseCalculating::last_time = 0;

AC_RET PoseCalculating::init()
{
    Framework::DeviceCaliData cali;
    Driver::Nvs *fd = nullptr;

    mailbox = Utils::Mailbox<Component::PoseData>::find("attitude");

    if (mailbox == nullptr)
    {
        BASE_ERROR("attitude mailbox not found");
        return AC_ERROR;
    }

    acc = Framework::VirtualDevice::find<Framework::Accelerometer>("acc", Framework::ACCELEROMETER_DEV);
    gyro = Framework::VirtualDevice::find<Framework::Gyroscope>("gyro", Framework::GYROSCOPE_DEV);

    if (acc == nullptr || gyro == nullptr)
    {
        BASE_ERROR("acc or gyro is nullptr");
        return AC_ERROR;
    }

    fd = Driver::Nvs::open("calibration");
    if (fd == nullptr)
    {
        BASE_ERROR("open calibration data failed");
        return AC_ERROR;
    }

    fd->read("gyro", &cali, sizeof(Framework::DeviceCaliData));
    gyro->setCali(cali);

    fd->read("acc", &cali, sizeof(Framework::DeviceCaliData));
    acc->setCali(cali);

    Driver::Nvs::close(fd);
    Component::Filter *gx_filter = new Component::SecondOrderLPF();
    gx_filter->init(1000.f, 100.f);
    Component::Filter *gy_filter = new Component::SecondOrderLPF();
    gy_filter->init(1000.f, 100.f);
    Component::Filter *gz_filter = new Component::SecondOrderLPF();
    gz_filter->init(1000.f, 100.f);
    Component::Filter *ax_filter = new Component::SecondOrderLPF();
    ax_filter->init(1000.f, 100.f);
    Component::Filter *ay_filter = new Component::SecondOrderLPF();
    ay_filter->init(1000.f, 100.f);
    Component::Filter *az_filter = new Component::SecondOrderLPF();
    az_filter->init(1000.f, 100.f);

    attitude = new Component::Attitude(new Component::Mahony(), gx_filter, gy_filter, gz_filter, ax_filter, ay_filter, az_filter);

    Utils::Notify::sub(ENTER_CALI_EVENT, suspend);

    Utils::Notify::sub(LEAVE_CALI_EVENT, resume);

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

    Component::MemsData mems{Component::MEMS_6_AXIS};

    acc->read(mems.acc);
    gyro->read(mems.gyro);

    uint64_t current_time = Driver::HardwareTimer::getCurrentTime();
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
