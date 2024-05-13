//
// Created by zhaohe on 2024/4/28.
//

#ifndef EXPECT_STATE_H_
#define EXPECT_STATE_H_

#include "type.h"

struct ExpectState
{
    Euler euler{};
    Vec3 ang_v{};
    float throttle = 0.f;
    float height_rate = 0.f;
};

#endif //EXPECT_STATE_H_
