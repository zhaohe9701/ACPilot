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
    if (nullptr == _physical_device) goto error;
    RETURN_CHECK(_physical_device->readThermometer(data));
    return AC_OK;
    error:
    return AC_ERROR;
}
