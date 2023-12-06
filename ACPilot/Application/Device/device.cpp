//
// Created by zhaohe on 2023/8/29.
//

#include "device.h"
#include "string.h"

bool Device::isMatch(const char *name)
{
    if (0 == strncmp(_name, name, DEVIE_NAME_LEN))
    {
        return true;
    }
    return false;
}