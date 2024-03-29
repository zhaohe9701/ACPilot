/*
 * @Description:
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-08-20 14:23:56
 * @LastEditors: zhaohe
 * @LastEditTime: 2022-12-25 01:44:24
 * @FilePath: \ZH_FLIGHT\Sys\Control\AttitudeControl\attitude_controller_interface.h
 * Copyright (C) 2022 zhaohe. All rights reserved.
 */
#ifndef ATTITUDE_CONTROLLER_H_
#define ATTITUDE_CONTROLLER_H_

#include "AircraftState/aircraft_state.h"
#include "Actuator/actuator_data.h"
#include <stdint.h>


class AttitudeController
{
public:
    virtual void init() = 0;
    virtual void update(AttitudeState &state, ActuatorData &actuator_value) = 0;
    virtual ~AttitudeController() = default;
};

#endif
