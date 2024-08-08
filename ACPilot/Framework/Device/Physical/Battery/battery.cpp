//
// Created by zhaohe on 2024/5/11.
//

#include "battery.h"

using namespace Framework;

AC_RET Battery::init()
{
    return AC_OK;
}

AC_RET Battery::getVoltage(VolData &data)
{
    return AC_NOT_SUPPORT;
}

AC_RET Battery::getCurrent(CurData &data)
{
    return AC_NOT_SUPPORT;
}

AC_RET Battery::getElectricQuantity(ElectricQuantityData &data)
{
    return AC_NOT_SUPPORT;
}

AC_RET Battery::updateVoltage()
{
    return AC_NOT_SUPPORT;
}

AC_RET Battery::updateCurrent()
{
    return AC_NOT_SUPPORT;
}

AC_RET Battery::updateElectricQuantity()
{
    return AC_NOT_SUPPORT;
}

AC_RET Battery::readVoltmeter(VolData &data)
{
    RETURN_CHECK(updateVoltage());
    RETURN_CHECK(getVoltage(data));
    return AC_OK;
    error:
    return AC_ERROR;
}

AC_RET Battery::readAmmeter(CurData &data)
{
    RETURN_CHECK(updateCurrent());
    RETURN_CHECK(getCurrent(data));
    return AC_OK;
    error:
    return AC_ERROR;
}

AC_RET Battery::readCoulometer(ElectricQuantityData &data)
{
    RETURN_CHECK(updateElectricQuantity());
    RETURN_CHECK(getElectricQuantity(data));
    return AC_OK;
    error:
    return AC_ERROR;
}


