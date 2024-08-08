//
// Created by zhaohe on 2024/5/9.
//

#ifndef ALTITUDE_CONTROL_H_
#define ALTITUDE_CONTROL_H_


#include "Type/type.h"
#include "Controller/expect_state.h"
#include "Mailbox/mailbox.h"
#include "Thread/thread.h"
#include "Device/device_data.h"
#include "Algorithm/AltitudeEstimate/altitude_estimate.h"
#include "AircraftState/aircraft_state.h"
#include "Controller/PID/pid.h"

class AltitudeControl
{
public:
    static AC_RET init();

    static void setAccBias(Common::Vec3 &acc_bias);

    static AC_RET step(Component::ExpectState &expect_state, AircraftState &state);

    static void reset();

    static void clear();

private:
    static void _altitudeSensorUpdate(void *param);

    static Utils::Mailbox<Framework::AltitudeData> *_baro_mailbox;
    static Osal::AcThread *_baro_task;
    static Component::AltitudeEstimate *_estimator;
    static Common::Vec3 _acc_bias;
    static float _height_bias;
    static float _aim_height;
    static float _base_throttle;
    static bool _isTakeOff;
    static uint64_t last_time;
    static Component::Pid *_vel_pid;
    static Component::Pid *_pos_pid;
};


#endif //ALTITUDE_CONTROL_H_
