//
// Created by zhaohe on 2024/4/17.
//

#ifndef ATTITUDE_H_
#define ATTITUDE_H_

#include "type.h"
#include "ahrs.h"
#include "attitude_solver.h"

class Attitude
{
public:
    explicit Attitude(AttitudeSolver *solver);

    AC_RET update(MemsData &mems_data, float dt);

    void reset();

    Euler euler;
    Vec3 ang_v;
private:
    AttitudeSolver *_solver = nullptr;
};

#endif //ATTITUDE_H_
