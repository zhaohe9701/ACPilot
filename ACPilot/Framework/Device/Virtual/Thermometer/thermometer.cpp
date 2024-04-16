//
// Created by zhaohe on 2023/8/30.
//

#include "thermometer.h"

Thermometer::Thermometer(const char *name) : VirtualDevice(name)
{
    _type = THERMOMETER_DEV;
}

AC_RET Thermometer::read(TempData &data)
{
    if (nullptr == _physical_device) return AC_ERROR;

    return _physical_device->readThermometer(data);
}