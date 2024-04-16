/*
 * @Description:
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-07-08 01:10:13
 * @LastEditors: zhaohe
 * @LastEditTime: 2023-04-10 23:04:37
 * @FilePath: \ZH_FLIGHT\Sys\Sensor\Imu\mpu6X00.h
 * Copyright (C) 2022 zhaohe. All rights reserved.
 */
#ifndef MPU6000_H_
#define MPU6000_H_

#include "tdk_imu.h"

class Mpu6X00 : virtual public TdkImu
{
public:
    explicit Mpu6X00(IoInterface *interface);

    AC_RET init() override;

    uint8_t getId() override;

    AC_RET getTemp(TempData &data) override;

    AC_RET getGyro(GyroData &data) override;

    AC_RET getAcc(AccData &data) override;

    AC_RET updateTemp() override;

    AC_RET updateGyro() override;

    AC_RET updateAcc() override;

private:

    float _acc_sensitivity = 0.0f;
    float _gyro_sensitivity = 0.0f;
    int16_t _bias_acc_x = 0;
    int16_t _bias_acc_y = 0;
    int16_t _bias_acc_z = 0;
    int16_t _bias_gyro_x = 0;
    int16_t _bias_gyro_y = 0;
    int16_t _bias_gyro_z = 0;
};

#endif