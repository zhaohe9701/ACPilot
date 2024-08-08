//
// Created by zhaohe on 2024/5/11.
//

#include "ammeter.h"

using namespace Framework;

Ammeter::Ammeter(const char *name) : VirtualDevice(name)
{
    _type = AMMETER_DEV;
}

AC_RET Ammeter::read(CurData &data)
{
    if (nullptr == _physical_device) goto error;
    RETURN_CHECK(_physical_device->readAmmeter(data));
    return AC_OK;
    error:
    return AC_ERROR;
}
