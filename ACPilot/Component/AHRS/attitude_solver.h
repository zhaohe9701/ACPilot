/*
 * @Description:
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-09-24 17:37:08
 * @LastEditors: zhaohe
 * @LastEditTime: 2023-04-09 23:23:43
 * @FilePath: \ZH_FLIGHT\Sys\AHRS\ahrs.h
 * Copyright (C) 2022 zhaohe. All rights reserved.
 */
#ifndef ATTITUDE_SOLVER_H_
#define ATTITUDE_SOLVER_H_

#include "ahrs.h"

class AttitudeSolver
{
public:
    virtual void update(MemsData &mems, Euler &euler, float dt) = 0;
    virtual void reset() = 0;
    virtual void transformBodyToEarth(Vec3 &vec);
    virtual void transformEarthToBody(Vec3 &vec);
    virtual ~AttitudeSolver()= default;

protected:
    float _r_mat[3][3] = {{0}};
};

#endif
