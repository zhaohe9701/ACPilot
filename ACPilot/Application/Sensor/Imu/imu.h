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

#include "Interface/device_interface.h"
#include "type.h"
#include "Device/accelerometer.h"
#include "Device/gyroscope.h"
#include "Device/magnetometer.h"
#include "Device/thermometer.h"
#include "Mutex/ac_mutex.h"

class ImuData
{
public:
    Vec3 acc;
    Vec3 gyr;
};

class Imu
        : virtual public Accelerometer,
          virtual public Gyroscope,
          virtual public Magnetometer,
          virtual public Thermometer
{
public:
    Imu() = default;

    virtual void init() = 0;

    virtual uint8_t getId() = 0;

    AC_RET readTmp(TmpData &data) override;

    AC_RET readGyro(GyroData &data) override;

    AC_RET readAcc(AccData &data) override;

    AC_RET readMag(MagData &data) override;

    virtual AC_RET updateTmp();

    virtual AC_RET updateGyro();

    virtual AC_RET updateAcc();

    virtual AC_RET updateMag();

protected:
    uint8_t _id = 0x00;
    uint8_t _READ = 0x00;
    uint8_t _WRITE = 0x00;
    uint8_t _BANK0 = 0x00;
    uint8_t _REG_BANK_SEL = 0x00;
    DeviceInterface *_interface = nullptr;

    TmpData _tmp_data{0};
    GyroData _gyro_data{0.0f, 0.0f, 0.0f};
    AccData _acc_data{0.0f, 0.0f, 0.0f};
    MagData _mag_data{0.0f, 0.0f, 0.0f};

    AcMutex _tmp_mutex{};
    AcMutex _gyro_mutex{};
    AcMutex _acc_mutex{};
    AcMutex _mag_mutex{};

    char _name[PARAM_NAME_LEN] = "imu";

    void _imuWriteRag(uint8_t bank, uint8_t address, uint8_t value);

    void _imuReadRag(uint8_t bank, uint8_t address, uint8_t length, uint8_t *buf);

    void _imuWriteRag(uint8_t address, uint8_t value);

    void _imuReadRag(uint8_t address, uint8_t length, uint8_t *buf);

};

#endif