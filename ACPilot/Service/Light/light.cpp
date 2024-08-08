//
// Created by zhaohe on 2024/4/19.
//

#include "light.h"
#include "light_server.h"

using namespace Service;

AC_RET Light::setMode(LightMode mode)
{
    return AC_NOT_SUPPORT;
}

bool Light::match(uint8_t id)
{
    if (id == _id)
    {
        return true;
    }
    return false;
}

Light::Light(uint8_t id)
{
    _id = id;
    LightServer::add(this);
}

