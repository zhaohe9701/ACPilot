//
// Created by zhaohe on 2024/4/29.
//

#ifndef POSE_CONTROL_H_
#define POSE_CONTROL_H_

#include "Controller/expect_state.h"
#include "Controller/PID/pid.h"
#include "AHRS/attitude.h"

class PoseControl
{
public:
    static AC_RET init();

    static AC_RET step(ExpectState &expect_state, PoseData &attitude, Vec3 &out);

    static void reset();

    static void clear();
private:
    static Pid *o_pitch;
    static Pid *o_roll;
    static Pid *o_yaw;

    static Pid *i_pitch;
    static Pid *i_roll;
    static Pid *i_yaw;

    static Filter *pitch_filter;
    static Filter *roll_filter;
    static Filter *yaw_filter;
    static uint64_t last_time;
    static void innerRing(ExpectState &expect_state, PoseData &attitude, Vec3 &out, float dt);

    static void outerRing(ExpectState &expect_state, PoseData &attitude, float dt);
};

#endif //POSE_CONTROL_H_
