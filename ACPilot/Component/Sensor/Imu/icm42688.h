/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2023-01-14 23:10:20
 * @LastEditors: zhaohe
 * @LastEditTime: 2023-04-10 22:44:57
 * @FilePath: \ZH_FLIGHT\Sys\Sensor\Imu\icm42688.h
 * Copyright (C) 2023 zhaohe. All rights reserved.
 */
#ifndef ICM42688_H_
#define ICM42688_H_

#include "tdk_imu.h"

namespace Component
{
    class Icm42688 : public TdkImu
    {
    public:
        explicit Icm42688(Interface::IO *interface);

        AC_RET init() override;

        uint8_t getId() override;

        AC_RET getTemp(Framework::TempData &data) override;

        AC_RET getGyro(Framework::GyroData &data) override;

        AC_RET getAcc(Framework::AccData &data) override;

        AC_RET updateTemp() override;

        AC_RET updateGyro() override;

        AC_RET updateAcc() override;
    };
}

#endif