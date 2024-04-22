//
// Created by zhaohe on 2024/4/17.
//
#include "attitude.h"

Attitude::Attitude(AttitudeSolver *solver)
{
    _solver = solver;
}

AC_RET Attitude::update(MemsData &mems_data, float dt)
{
    _solver->update(mems_data, euler, dt);
    ang_v.x = mems_data.gyro.x;
    ang_v.y = mems_data.gyro.y;
    ang_v.z = mems_data.gyro.z;
    return AC_OK;
}

void Attitude::reset()
{
    _solver->reset();
    euler.clear();
    ang_v.clear();
}


