//
// Created by zhaohe on 2024/5/9.
//

#include "variance.h"

using namespace Component;

AC_RET Variance::calculate(Common::Vec3 *data, uint16_t num, Common::Vec3 &variance)
{
    Common::Vec3 avg;
    if (nullptr == data)
    {
        return AC_ERROR;
    }
    variance.clear();
    for (uint16_t i = 0; i < num; i++)
    {
        avg.x += data[i].x;
        avg.y += data[i].y;
        avg.z += data[i].z;
    }
    avg.x /= (float)num;
    avg.y /= (float)num;
    avg.z /= (float)num;
    for (uint16_t i = 0; i < num; i++)
    {
        variance.x += (data[i].x - avg.x) * (data[i].x - avg.x);
        variance.y += (data[i].y - avg.y) * (data[i].y - avg.y);
        variance.z += (data[i].z - avg.z) * (data[i].z - avg.z);
    }
    variance.x /= (float)num;
    variance.y /= (float)num;
    variance.z /= (float)num;
    return AC_OK;
}
