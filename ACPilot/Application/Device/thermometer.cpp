//
// Created by zhaohe on 2023/8/30.
//

#include "thermometer.h"
#include "string.h"

Thermometer::Thermometer()
{
    _type = THERMOMETER_DEV;
}
void Thermometer::setName(const char *name)
{
    strncpy(_name, name, DEVIE_NAME_LEN - 1);
}

AC_RET Thermometer::read(TmpData &data)
{
    return readTmp(data);
}