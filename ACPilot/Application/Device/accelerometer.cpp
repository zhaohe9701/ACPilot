//
// Created by zhaohe on 2023/8/29.
//

#include "accelerometer.h"
#include "string.h"

Accelerometer::Accelerometer()
{
    _type = ACCELEROMETER_DEV;
}

void Accelerometer::setName(const char *name)
{
    strncpy(_name, name, DEVIE_NAME_LEN - 1);
}

AC_RET Accelerometer::read(AccData &data)
{
    return readAcc(data);
}