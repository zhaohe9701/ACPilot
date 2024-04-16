/*
 * @Description:
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-07-07 23:41:39
 * @LastEditors: zhaohe
 * @LastEditTime: 2023-04-10 23:04:02
 * @FilePath: \ZH_FLIGHT\Sys\Sensor\Imu\imu.h
 * Copyright (C) 2022 zhaohe. All rights reserved.
 */

#ifndef IMU_H_
#define IMU_H_

#include "IO/io_interface.h"
#include "Mutex/ac_mutex.h"
#include "Device/device_data.h"
#include "Device/Physical/physical_device.h"


class Imu : public PhysicalDevice
{
public:
    Imu() = default;

    virtual AC_RET init();

    virtual uint8_t getId();

    virtual AC_RET getTemp(TempData &data);

    virtual AC_RET getGyro(GyroData &data);

    virtual AC_RET getAcc(AccData &data);

    virtual AC_RET getMag(MagData &data);

    virtual AC_RET updateTemp();

    virtual AC_RET updateGyro();

    virtual AC_RET updateAcc();

    virtual AC_RET updateMag();

    AC_RET readAccelerometer(AccData &data) override;

    AC_RET readGyroscope(GyroData &data) override;

    AC_RET readMagnetometer(MagData &data) override;

    AC_RET readThermometer(TempData &data) override;

protected:
    uint8_t _id = 0x00;
    IoInterface *_interface = nullptr;
    char _name[PARAM_NAME_LEN] = "imu";

    TempData _tmp_data{0};
    GyroData _gyro_data{0.0f, 0.0f, 0.0f};
    AccData _acc_data{0.0f, 0.0f, 0.0f};
    MagData _mag_data{0.0f, 0.0f, 0.0f};
};

#endif