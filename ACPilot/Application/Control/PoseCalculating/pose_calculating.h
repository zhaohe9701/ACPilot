//
// Created by zhaohe on 2024/4/18.
//

#ifndef POSE_CALCULATING_H_
#define POSE_CALCULATING_H_

#include "type.h"
#include "AHRS/attitude.h"
#include "Mail/mailbox.h"
#include "Thread/ac_thread.h"
#include "Device/Virtual/Accelerometer/accelerometer.h"
#include "Device/Virtual/Gyroscope/gyroscope.h"
#include "AircraftState/aircraft_state.h"

class PoseCalculating
{
public:
    static AC_RET init();

    static AC_RET step(AircraftState &state);

    static volatile bool run;
private:

    static Mailbox<PoseData> *mailbox;
    static Accelerometer *acc;
    static Gyroscope *gyro;
    static Attitude *attitude;
    static uint64_t last_time;
};

#endif //POSE_CALCULATING_H_
