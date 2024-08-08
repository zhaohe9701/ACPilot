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

#include "tdk_imu.h"

namespace Component
{
    class Icm20689 : public TdkImu
    {
    public:
        explicit Icm20689(Interface::IO *interface);

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