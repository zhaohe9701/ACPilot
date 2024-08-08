//
// Created by zhaohe on 2024/4/16.
//

#include "mag.h"
#include "error_handing.h"

using namespace Framework;

AC_RET Mag::init()
{
    return AC_OK;
}

AC_RET Mag::getMag(MagData &data)
{
    return AC_NOT_SUPPORT;
}

AC_RET Mag::updateMag()
{
    return AC_NOT_SUPPORT;
}

AC_RET Mag::readMagnetometer(MagData &data)
{
    RETURN_CHECK(updateMag());
    RETURN_CHECK(getMag(data));
    return AC_OK;
    error:
    return AC_ERROR;
}
