//
// Created by zhaohe on 2024/4/17.
//
#include "attitude.h"

using namespace Component;

Attitude::Attitude(AttitudeSolver *solver, Filter *gx_filter
                                         , Filter *gy_filter
                                         , Filter *gz_filter
                                         , Filter *ax_filter
                                         , Filter *ay_filter
                                         , Filter *az_filter)
{
    _solver = solver;
    _gx_filter = gx_filter;
    _gy_filter = gy_filter;
    _gz_filter = gz_filter;
    _ax_filter = ax_filter;
    _ay_filter = ay_filter;
    _az_filter = az_filter;
}

AC_RET Attitude::update(MemsData &mems_data, float dt)
{
    if (_solver == nullptr)
    {
        return AC_ERROR;
    }
    if (nullptr != _gx_filter)
    {
        mems_data.gyro.x = _gx_filter->apply(mems_data.gyro.x);
        pose.ang_v.x = mems_data.gyro.x;
    } else
    {
        pose.ang_v.x = mems_data.gyro.x;
    }
    if (nullptr != _gy_filter)
    {
        mems_data.gyro.y = _gy_filter->apply(mems_data.gyro.y);
        pose.ang_v.y = mems_data.gyro.y;
    } else
    {
        pose.ang_v.y = mems_data.gyro.y;
    }
    if (nullptr != _gz_filter)
    {
        mems_data.gyro.z = _gz_filter->apply(mems_data.gyro.z);
        pose.ang_v.z = mems_data.gyro.z;
    } else
    {
        pose.ang_v.z = mems_data.gyro.z;
    }
    if (nullptr != _ax_filter)
    {
        mems_data.acc.x = _ax_filter->apply(mems_data.acc.x);
    }
    if (nullptr != _ay_filter)
    {
        mems_data.acc.y = _ay_filter->apply(mems_data.acc.y);
    }
    if (nullptr != _az_filter)
    {
        mems_data.acc.z = _az_filter->apply(mems_data.acc.z);
    }
    _solver->update(mems_data, pose.euler, dt);
    earth_acc = mems_data.acc;
    _solver->transformBodyToEarth(earth_acc);
    return AC_OK;
}

void Attitude::reset()
{
    _solver->reset();
    pose.euler.clear();
    pose.ang_v.clear();
}


