//
// Created by zhaohe on 2024/4/17.
//

#include "barometer.h"

Barometer::Barometer(const char *name) : VirtualDevice(name)
{
    _type = BAROMETER_DEV;
}


AC_RET Barometer::read(AtaData &data)
{
    if (nullptr == _physical_device) goto error;
    RETURN_CHECK(_physical_device->readBarometer(data))
    return AC_OK;
    error:
    return AC_ERROR;
}

