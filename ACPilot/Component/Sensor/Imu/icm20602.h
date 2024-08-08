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

#include "tdk_imu.h"

namespace Component
{
    class Icm20602 : public TdkImu
    {
    public:
        explicit Icm20602(Interface::IO *interface);

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
