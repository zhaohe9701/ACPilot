//
// Created by zhaohe on 2024/4/17.
//

#include "altimeter.h"

using namespace Framework;

Altimeter::Altimeter(const char *name) : VirtualDevice(name)
{
    _type = ALTIMETER_DEV;
}


AC_RET Altimeter::read(AltitudeData &data)
{
    if (nullptr == _physical_device) goto error;
    RETURN_CHECK(_physical_device->readAltimeter(data))
    return AC_OK;
    error:
    return AC_ERROR;
}

