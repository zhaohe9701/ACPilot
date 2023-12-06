/*
 * @Description:
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-09-24 17:37:08
 * @LastEditors: zhaohe
 * @LastEditTime: 2023-04-09 23:23:43
 * @FilePath: \ZH_FLIGHT\Sys\AHRS\ahrs.h
 * Copyright (C) 2022 zhaohe. All rights reserved.
 */
#ifndef AHRS_H_
#define AHRS_H_

#include "AircraftState/aircraft_state.h"
#include "Sensor/sensor.h"
#include "config.h"

class AttitudeSolver
{
public:
    virtual void update(ActualAttitudeState &actual_state, SensorData &sensor_data) = 0;
    virtual void transformBodyToEarth(float &x, float &y, float &z) = 0;
    virtual void transformEarthToBody(float &x, float &y, float &z) = 0;
    virtual ~AttitudeSolver()= default;
};

#endif
