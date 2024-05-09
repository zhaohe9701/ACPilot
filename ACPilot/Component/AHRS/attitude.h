//
// Created by zhaohe on 2024/4/17.
//

#ifndef ATTITUDE_H_
#define ATTITUDE_H_

#include "type.h"
#include "ahrs.h"
#include "attitude_solver.h"
#include "Filter/filter.h"

struct PoseData
{
    Euler euler;
    Vec3 ang_v;
};

class Attitude
{
public:
    Attitude() = default;

    Attitude(AttitudeSolver *solver, Filter *gx_filter, Filter *gy_filter, Filter *gz_filter, Filter *ax_filter,
             Filter *ay_filter, Filter *az_filter);

    AC_RET update(MemsData &mems_data, float dt);

    void reset();

    PoseData pose;

    Vec3 earth_acc;
private:
    AttitudeSolver *_solver = nullptr;
    Filter *_gx_filter = nullptr;
    Filter *_gy_filter = nullptr;
    Filter *_gz_filter = nullptr;

    Filter *_ax_filter = nullptr;
    Filter *_ay_filter = nullptr;
    Filter *_az_filter = nullptr;
};

#endif //ATTITUDE_H_
