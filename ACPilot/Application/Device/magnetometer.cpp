//
// Created by zhaohe on 2023/8/30.
//

#include "magnetometer.h"
#include "string.h"

Magnetometer::Magnetometer()
{
    _type = MAGNETOMETER_DEV;
}
void Magnetometer::setName(const char *name)
{
    strncpy(_name, name, DEVIE_NAME_LEN - 1);
}

AC_RET Magnetometer::read(MagData &data)
{
    return readMag(data);
}