/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-08-07 22:30:11
 * @LastEditors: zhaohe
 * @LastEditTime: 2023-03-02 00:23:50
 * @FilePath: \ZH_FLIGHT\Sys\AircraftState\aircraft_state.h
 * Copyright (C) 2022 zhaohe. All rights reserved.
 */
#ifndef AIRCRAFT_STATE_H_
#define AIRCRAFT_STATE_H_

#include "Type/type.h"
#include "AHRS/attitude.h"

struct AircraftState
{
    Component::PoseData pose;
    Common::Vec3 earth_acc;
    Common::Vec3 earth_vel;
    Common::Vec3 earth_pos;
};

#endif

