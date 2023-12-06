/*
 * @Description:
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-11-09 22:39:24
 * @LastEditors: zhaohe
 * @LastEditTime: 2023-04-10 22:45:45
 * @FilePath: \ZH_FLIGHT\Sys\Sensor\Imu\icm20602.h
 * Copyright (C) 2022 zhaohe. All rights reserved.
 */
#ifndef ICM20602_H_
#define ICM20602_H_

#include "imu.h"

class Icm20602 : virtual public Imu
{
public:
    Icm20602(DeviceInterface *interface);

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
