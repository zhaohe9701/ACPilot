//
// Created by zhaohe on 2024/4/29.
//

#include "pose_control.h"
#include "Controller/PID/pid.h"
#include "dm_template.h"
#include "DataModule/data_module.h"
#include "error_handing.h"
#include "default_debug.h"
#include "AHRS/attitude.h"
#include "Mail/mailbox.h"
#include "Filter/IIR/lpf.h"
#include "HardTimer/hard_timer_driver.h"

Pid *PoseControl::o_pitch = nullptr;
Pid *PoseControl::o_roll = nullptr;
Pid *PoseControl::o_yaw = nullptr;

Pid *PoseControl::i_pitch = nullptr;
Pid *PoseControl::i_roll = nullptr;
Pid *PoseControl::i_yaw = nullptr;

Filter *PoseControl::pitch_filter = nullptr;
Filter *PoseControl::roll_filter = nullptr;
Filter *PoseControl::yaw_filter = nullptr;

uint64_t PoseControl::last_time = 0;

static void poseControlReset(void *arg)
{
    PoseControl::reset();
}

void PoseControl::innerRing(ExpectState &expect_state, PoseData &attitude, Vec3 &out, float dt)
{

    out.y = i_pitch->output(expect_state.ang_v.y - attitude.ang_v.y, dt);
    out.x = i_roll->output(expect_state.ang_v.x - attitude.ang_v.x, dt);
    out.z = i_yaw->output(expect_state.ang_v.z - attitude.ang_v.z, dt);
}

void PoseControl::outerRing(ExpectState &expect_state, PoseData &attitude, float dt)
{
    expect_state.ang_v.y = o_pitch->output(expect_state.euler.pitch - attitude.euler.pitch, dt);
    expect_state.ang_v.x = o_roll->output(expect_state.euler.roll - attitude.euler.roll, dt);
    float yaw_error = expect_state.euler.yaw - attitude.euler.yaw;
    if (yaw_error > 180.0f)
        yaw_error -= 360.0f;
    else if (yaw_error < -180.0f)
        yaw_error += 360.0f;
    expect_state.ang_v.z = o_yaw->output(yaw_error, dt);
}

AC_RET PoseControl::init()
{
    PidConfig config;

    pitch_filter = new FirstOrderLPF();
    roll_filter = new FirstOrderLPF();
    yaw_filter = new FirstOrderLPF();

    o_pitch = new Pid();
    o_roll = new Pid();
    o_yaw = new Pid();

    i_pitch = new Pid(pitch_filter);
    i_roll = new Pid(roll_filter);
    i_yaw = new Pid(yaw_filter);

    RETURN_CHECK(DataModule::read("/controller/pose[4]/1", &config, sizeof(PidConfig)));
    o_pitch->init(config.kp, config.ki, config.kd, config.out_limit, config.int_limit);
    RETURN_CHECK(DataModule::read("/controller/pose[4]/1", &config, sizeof(PidConfig)));
    o_roll->init(config.kp, config.ki, config.kd, config.out_limit, config.int_limit);
    RETURN_CHECK(DataModule::read("/controller/pose[4]/2", &config, sizeof(PidConfig)));
    o_yaw->init(config.kp, config.ki, config.kd, config.out_limit, config.int_limit);

    RETURN_CHECK(DataModule::read("/controller/pose[4]/3", &config, sizeof(PidConfig)));
    i_pitch->init(config.kp, config.ki, config.kd, config.out_limit, config.int_limit);
    pitch_filter->init(1000, config.d_filter_cut_freq);
    RETURN_CHECK(DataModule::read("/controller/pose[4]/3", &config, sizeof(PidConfig)));
    i_roll->init(config.kp, config.ki, config.kd, config.out_limit, config.int_limit);
    roll_filter->init(1000, config.d_filter_cut_freq);
    RETURN_CHECK(DataModule::read("/controller/pose[4]/4", &config, sizeof(PidConfig)));
    i_yaw->init(config.kp, config.ki, config.kd, config.out_limit, config.int_limit);
    yaw_filter->init(1000, config.d_filter_cut_freq);

    RETURN_CHECK(DataModule::registerAction("/controller/pose_reset", poseControlReset));

    return AC_OK;

    error:
    BASE_ERROR("pose control init failed");
    return AC_ERROR;
}

AC_RET PoseControl::step(ExpectState &expect_state, PoseData &attitude, Vec3 &out)
{
    uint64_t current_time = HardwareTimer::getCurrentTime();
    if (last_time == 0)
    {
        last_time = current_time;
        return AC_OK;
    }
    uint32_t delta_time = current_time - last_time;
    last_time = current_time;
    outerRing(expect_state, attitude, US_TO_S(delta_time));
    innerRing(expect_state, attitude, out, US_TO_S(delta_time));
    return AC_OK;
}

void PoseControl::reset()
{
    PidConfig config;
    RETURN_CHECK(DataModule::read("/controller/pose[4]/1", &config, sizeof(PidConfig)));
    o_pitch->init(config.kp, config.ki, config.kd, config.out_limit, config.int_limit);
    RETURN_CHECK(DataModule::read("/controller/pose[4]/1", &config, sizeof(PidConfig)));
    o_roll->init(config.kp, config.ki, config.kd, config.out_limit, config.int_limit);
    RETURN_CHECK(DataModule::read("/controller/pose[4]/2", &config, sizeof(PidConfig)));
    o_yaw->init(config.kp, config.ki, config.kd, config.out_limit, config.int_limit);

    RETURN_CHECK(DataModule::read("/controller/pose[4]/3", &config, sizeof(PidConfig)));
    i_pitch->init(config.kp, config.ki, config.kd, config.out_limit, config.int_limit);
    pitch_filter->init(1000, config.d_filter_cut_freq);
    RETURN_CHECK(DataModule::read("/controller/pose[4]/3", &config, sizeof(PidConfig)));
    i_roll->init(config.kp, config.ki, config.kd, config.out_limit, config.int_limit);
    roll_filter->init(1000, config.d_filter_cut_freq);
    RETURN_CHECK(DataModule::read("/controller/pose[4]/4", &config, sizeof(PidConfig)));
    i_yaw->init(config.kp, config.ki, config.kd, config.out_limit, config.int_limit);
    yaw_filter->init(1000, config.d_filter_cut_freq);
    return;
    error:
    BASE_ERROR("pose control reset failed");
}

void PoseControl::clear()
{
    o_pitch->clear();
    o_roll->clear();
    i_pitch->clear();
    i_roll->clear();
    i_yaw->clear();
}
