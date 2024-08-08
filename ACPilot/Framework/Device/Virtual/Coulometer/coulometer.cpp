//
// Created by zhaohe on 2024/5/11.
//

#include "coulometer.h"

using namespace Framework;

Coulometer::Coulometer(const char *name) : VirtualDevice(name)
{
    _type = COULOMETER_DEV;
}

AC_RET Coulometer::read(ElectricQuantityData &data)
{
    if (nullptr == _physical_device) goto error;
    RETURN_CHECK(_physical_device->readCoulometer(data));
    return AC_OK;
    error:
    return AC_ERROR;
}
