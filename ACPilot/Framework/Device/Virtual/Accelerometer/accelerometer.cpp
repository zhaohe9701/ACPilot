//
// Created by zhaohe on 2023/8/29.
//

#include "accelerometer.h"

Accelerometer::Accelerometer(const char *name) : VirtualDevice(name)
{
    _type = ACCELEROMETER_DEV;
}

AC_RET Accelerometer::read(AccData &data)
{
    if (nullptr == _physical_device) return AC_ERROR;

    return _physical_device->readAccelerometer(data);
}