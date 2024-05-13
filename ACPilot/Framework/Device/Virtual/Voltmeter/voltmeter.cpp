//
// Created by zhaohe on 2024/5/11.
//

#include "voltmeter.h"

Voltmeter::Voltmeter(const char *name) : VirtualDevice(name)
{
    _type = VOLTMETER_DEV;
}

AC_RET Voltmeter::read(VolData &data)
{
    if (nullptr == _physical_device) goto error;
    RETURN_CHECK(_physical_device->readVoltmeter(data));
    return AC_OK;
    error:
    return AC_ERROR;
}
