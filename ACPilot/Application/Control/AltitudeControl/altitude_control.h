//
// Created by zhaohe on 2024/5/9.
//

#ifndef ALTITUDE_CONTROL_H_
#define ALTITUDE_CONTROL_H_


#include "type.h"
#include "Controller/expect_state.h"
#include "Mail/mailbox.h"
#include "Thread/ac_thread.h"
#include "Device/device_data.h"
#include "Algorithm/AltitudeEstimate/altitude_estimate.h"
#include "AircraftState/aircraft_state.h"

class AltitudeControl
{
public:
    static AC_RET init();

    static void setAccBias(Vec3 &acc_bias);

    static AC_RET step(ExpectState &expect_state, AircraftState &state);

    static void reset();

    static void clear();
private:
    static void _altitudeSensorUpdate(void *param);

    static Mailbox<AltitudeData> *_baro_mailbox;

    static AcThread *_baro_task;

    static AltitudeEstimate *_estimator;

    static Vec3 _acc_bias;

    static float _height_bias;

    static uint64_t last_time;
};


#endif //ALTITUDE_CONTROL_H_
