//
// Created by zhaohe on 2023/4/20.
//
#include "baro.h"
#include "error_handing.h"

using namespace Framework;

AC_RET Baro::init()
{
    return AC_OK;
}

AC_RET Baro::getTemp(TempData &data)
{
    return AC_NOT_SUPPORT;
}

AC_RET Baro::getPressure(AtaData &data)
{
    return AC_NOT_SUPPORT;
}

AC_RET Baro::getAltitude(AltitudeData &data)
{
    return AC_NOT_SUPPORT;
}

AC_RET Baro::updateTemp()
{
    return AC_NOT_SUPPORT;
}

AC_RET Baro::updatePressure()
{
    return AC_NOT_SUPPORT;
}

AC_RET Baro::updateAltitude()
{
    return AC_NOT_SUPPORT;
}

AC_RET Baro::readAltimeter(AltitudeData &data)
{
    RETURN_CHECK(updateAltitude());
    RETURN_CHECK(getAltitude(data));
    return AC_OK;
    error:
    return AC_ERROR;
}

AC_RET Baro::readThermometer(TempData &data)
{
    RETURN_CHECK(updateTemp());
    RETURN_CHECK(getTemp(data));
    return AC_OK;
    error:
    return AC_ERROR;
}

AC_RET Baro::readBarometer(AtaData &data)
{
    RETURN_CHECK(updatePressure());
    RETURN_CHECK(getPressure(data));
    return AC_OK;
    error:
    return AC_ERROR;
}
