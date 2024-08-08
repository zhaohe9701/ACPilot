//
// Created by zhaohe on 2024/4/18.
//

#ifndef POSE_CALCULATING_H_
#define POSE_CALCULATING_H_

#include "Type/type.h"
#include "AHRS/attitude.h"
#include "Mailbox/mailbox.h"
#include "Thread/thread.h"
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

    static Utils::Mailbox<Component::PoseData> *mailbox;
    static Framework::Accelerometer *acc;
    static Framework::Gyroscope *gyro;
    static Component::Attitude *attitude;
    static uint64_t last_time;
};

#endif //POSE_CALCULATING_H_
