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

#include "type.h"
#include "AHRS/attitude.h"

struct AircraftState
{
    PoseData pose;
    Vec3 earth_acc;
    Vec3 earth_vel;
    Vec3 earth_pos;
};

#endif

