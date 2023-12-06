//
// Created by zhaohe on 2023/8/30.
//

#include "gyroscope.h"
#include "string.h"

Gyroscope::Gyroscope()
{
    _type = GYROSCOPE_DEV;
}
void Gyroscope::setName(const char *name)
{
    strncpy(_name, name, DEVIE_NAME_LEN - 1);
}

AC_RET Gyroscope::read(GyroData &data)
{
    return readGyro(data);
}