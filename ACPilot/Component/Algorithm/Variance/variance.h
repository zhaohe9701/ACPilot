//
// Created by zhaohe on 2024/5/9.
//

#ifndef VARIANCE_H_
#define VARIANCE_H_

#include "Type/type.h"

namespace Component
{
    class Variance
    {
    public:
        static AC_RET calculate(Common::Vec3 *data, uint16_t num, Common::Vec3 &variance);
    };
}

#endif //VARIANCE_H_
