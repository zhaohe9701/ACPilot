/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2023-04-10 22:48:12
 * @LastEditors: zhaohe
 * @LastEditTime: 2023-04-11 00:34:12
 * @FilePath: \ZH_FLIGHT\Sys\Sensor\Imu\imu.cpp
 * Copyright (C) 2023 zhaohe. All rights reserved.
 */
#include "imu.h"
#include "error_handing.h"

AC_RET Imu::init()
{
    return AC_OK;
}

uint8_t Imu::getId()
{
    return 0x0;
}

AC_RET Imu::getTemp(TempData &data)
{
    return AC_NOT_SUPPORT;
}

AC_RET Imu::getGyro(GyroData &data)
{
    return AC_NOT_SUPPORT;
}

AC_RET Imu::getAcc(AccData &data)
{
    return AC_NOT_SUPPORT;
}

AC_RET Imu::getMag(MagData &data)
{
    return AC_NOT_SUPPORT;
}

AC_RET Imu::updateTemp()
{
    return AC_NOT_SUPPORT;
}

AC_RET Imu::updateGyro()
{
    return AC_NOT_SUPPORT;
}

AC_RET Imu::updateAcc()
{
    return AC_NOT_SUPPORT;
}

AC_RET Imu::updateMag()
{
    return AC_NOT_SUPPORT;
}

AC_RET Imu::readAccelerometer(AccData &data)
{
    RETURN_CHECK(updateAcc());
    RETURN_CHECK(getAcc(data));
    return AC_OK;
    error:
    return AC_ERROR;
}

AC_RET Imu::readGyroscope(GyroData &data)
{
    RETURN_CHECK(updateGyro());
    RETURN_CHECK(getGyro(data));
    return AC_OK;
    error:
    return AC_ERROR;
}

AC_RET Imu::readMagnetometer(MagData &data)
{
    RETURN_CHECK(updateMag());
    RETURN_CHECK(getMag(data));
    return AC_OK;
    error:
    return AC_ERROR;
}

AC_RET Imu::readThermometer(TempData &data)
{
    RETURN_CHECK(updateTemp());
    RETURN_CHECK(getTemp(data));
    return AC_OK;
    error:
    return AC_ERROR;
}






