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

    static AC_RET step(Component::ExpectState &expect_state, Component::PoseData &attitude, Common::Vec3 &out);

    static void reset();

    static void clear();
private:
    static Component::Pid *o_pitch;
    static Component::Pid *o_roll;
    static Component::Pid *o_yaw;

    static Component::Pid *i_pitch;
    static Component::Pid *i_roll;
    static Component::Pid *i_yaw;

    static Component::Filter *pitch_filter;
    static Component::Filter *roll_filter;
    static Component::Filter *yaw_filter;
    static uint64_t last_time;
    static void innerRing(Component::ExpectState &expect_state, Component::PoseData &attitude, Common::Vec3 &out, float dt);

    static void outerRing(Component::ExpectState &expect_state, Component::PoseData &attitude, float dt);
};

#endif //POSE_CONTROL_H_
