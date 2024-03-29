/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-09-24 17:52:30
 * @LastEditors: zhaohe
 * @LastEditTime: 2022-12-19 23:51:02
 * @FilePath: \ZH_FLIGHT\Sys\AHRS\mahony.h
 * Copyright (C) 2022 zhaohe. All rights reserved.
 */
#ifndef MAHONY_H_
#define MAHONY_H_

#include "ahrs.h"


class Mahony : virtual public AttitudeSolver
{
public:
    void update(ActualAttitudeState &actual_state, SensorData &sensor_data) override;
    void transformBodyToEarth(float &x, float &y, float &z) override;
    void transformEarthToBody(float &x, float &y, float &z) override;
private:
    float _invSqrt(float x);
    void _computeRotationMatrix();

    float _kp = 0.4f;	
    float _ki = 0.001f;	
    float _ex_int = 0.0f;
    float _ey_int = 0.0f;
    float _ez_int = 0.0f;		


    float _q0 = 1.0f;	
    float _q1 = 0.0f;
    float _q2 = 0.0f;
    float _q3 = 0.0f;	

    float _dt = 1.0f / (float)ATTITUDE_SOLVE_CYCLE_FREQ;
    float _r_mat[3][3] = {{0}};
};

#endif
