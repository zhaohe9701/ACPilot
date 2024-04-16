//
// Created by zhaohe on 2024/4/12.
//

#include "physical_device.h"

AC_RET PhysicalDevice::readAccelerometer(AccData &data)
{
    return AC_ERROR;
}

AC_RET PhysicalDevice::readGyroscope(GyroData &data)
{
    return AC_ERROR;
}

AC_RET PhysicalDevice::readMagnetometer(MagData &data)
{
    return AC_ERROR;
}

AC_RET PhysicalDevice::readThermometer(TempData &data)
{
    return AC_ERROR;
}

AC_RET PhysicalDevice::readAltimeter(AltitudeData &data)
{
    return AC_ERROR;
}

AC_RET PhysicalDevice::readBarometer(AtaData &data)
{
    return AC_ERROR;
}

bool PhysicalDevice::haveAbility(VirtualDeviceType type)
{
    if (0 != (_ability & ((uint32_t)1 << type)))
    {
        return true;
    }
    return false;
}



