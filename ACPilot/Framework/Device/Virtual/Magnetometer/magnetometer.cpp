//
// Created by zhaohe on 2023/8/30.
//

#include "magnetometer.h"
#include "string.h"

Magnetometer::Magnetometer(const char *name) : VirtualDevice(name)
{
    _type = MAGNETOMETER_DEV;
}

AC_RET Magnetometer::read(MagData &data)
{
    if (nullptr == _physical_device) return AC_ERROR;

    return _physical_device->readMagnetometer(data);
}