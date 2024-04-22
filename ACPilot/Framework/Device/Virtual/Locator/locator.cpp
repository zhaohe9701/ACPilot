//
// Created by zhaohe on 2023/8/30.
//

#include "locator.h"

Locator::Locator(const char *name) : VirtualDevice(name)
{
    _type = LOCATOR_DEV;
}

AC_RET Locator::read(PosData &data)
{
    if (nullptr == _physical_device) goto error;
    RETURN_CHECK(_physical_device->readLocator(data));
    return AC_OK;
    error:
    return AC_ERROR;
}
