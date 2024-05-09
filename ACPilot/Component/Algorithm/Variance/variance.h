//
// Created by zhaohe on 2024/5/9.
//

#ifndef VARIANCE_H_
#define VARIANCE_H_


#include "type.h"

class Variance
{
public:
    static AC_RET calculate(Vec3 *data, uint16_t num, Vec3 &variance);
};


#endif //VARIANCE_H_
