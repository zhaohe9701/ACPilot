//
// Created by zhaohe on 2023/8/30.
//

#include "gyroscope.h"

Gyroscope::Gyroscope(const char *name) : VirtualDevice(name)
{
    _type = GYROSCOPE_DEV;
}

AC_RET Gyroscope::read(GyroData &data)
{
    if (nullptr == _physical_device) return AC_ERROR;

    return _physical_device->readGyroscope(data);
}