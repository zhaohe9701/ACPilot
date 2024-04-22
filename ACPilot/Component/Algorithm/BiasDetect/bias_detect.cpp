//
// Created by zhaohe on 2024/4/20.
//


#include "bias_detect.h"

void BiasDetect::cali(Vec3 *data, uint16_t num, DeviceCaliData &result)
{
    for (int i = 0; i < num; i++)
    {
        result.offset.x += data[i].x;
        result.offset.y += data[i].y;
        result.offset.z += data[i].z;
    }
    result.offset.x /= (float)num;
    result.offset.y /= (float)num;
    result.offset.z /= (float)num;
}
