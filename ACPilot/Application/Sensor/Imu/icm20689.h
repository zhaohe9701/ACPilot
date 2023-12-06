/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2023-01-14 23:10:20
 * @LastEditors: zhaohe
 * @LastEditTime: 2023-04-10 22:45:38
 * @FilePath: \ZH_FLIGHT\Sys\Sensor\Imu\icm20689.h
 * Copyright (C) 2023 zhaohe. All rights reserved.
 */
#ifndef ICM20689_H_
#define ICM20689_H_

#include "imu.h"

class Icm20689 : virtual public Imu
{
public:
    Icm20689(DeviceInterface *interface);

    void init() override;

    uint8_t getId() override;

    AC_RET readTmp(TmpData &data) override;

    AC_RET readGyro(GyroData &data) override;

    AC_RET readAcc(AccData &data) override;

    AC_RET updateTmp() override;

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