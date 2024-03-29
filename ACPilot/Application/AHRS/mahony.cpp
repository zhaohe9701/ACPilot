/*
 * @Description:
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-09-24 17:52:11
 * @LastEditors: zhaohe
 * @LastEditTime: 2023-04-09 23:24:10
 * @FilePath: \ZH_FLIGHT\Sys\AHRS\mahony.cpp
 * Copyright (C) 2022 zhaohe. All rights reserved.
 */
#include "mahony.h"
#include "Math/math_param.h"

void Mahony::update(ActualAttitudeState &actual_state, SensorData &sensor_data)
{
    float normalise;
    float ex = 0.0f;
    float ey = 0.0f;
    float ez = 0.0f;
    float gx = sensor_data.gyr.x * DEG2RAD;
    float gy = sensor_data.gyr.y * DEG2RAD;
    float gz = sensor_data.gyr.z * DEG2RAD;
    float ax = sensor_data.acc.x;
    float ay = sensor_data.acc.y;
    float az = sensor_data.acc.z;
    float halfT = 0.5f * _dt;
    float q0_last = 0.0f;
    float q1_last = 0.0f;
    float q2_last = 0.0f;
    float q3_last = 0.0f;

    actual_state.ang_acc.Set(sensor_data.gyr.y, sensor_data.gyr.x, sensor_data.gyr.z);

#ifdef MAGNETOMETER
    float mx = sensor_data.mag.x;
    float my = sensor_data.mag.y;
    float mz = sensor_data.mag.z;

    float hx = 0.0f;
    float hy = 0.0f;
    float hz = 0.0f;

    float bx = 0.0f;
    float bz = 0.0f;

    float wx = 0.0f;
    float wy = 0.0f;
    float wz = 0.0f;

    if (((mx != 0.0f) || (my != 0.0f) || (mz != 0.0f)) && ((ax != 0.0f) || (ay != 0.0f) || (az != 0.0f)))
    {
        normalise = _InvSqrt(ax * ax + ay * ay + az * az);
        ax *= normalise;
        ay *= normalise;
        az *= normalise;

        normalise = _InvSqrt(mx * mx + my * my + mz * mz);
        mx *= normalise;
        my *= normalise;
        mz *= normalise;

        hx = mx;
        hy = my;
        hz = mz;

        TransformBodyToEarth(hx, hy, hz);

        bx = sqrt(hx * hx + hy * hy);
        bz = hz;
        // bz = mx * _r_mat[2][0] + my * _r_mat[2][1] + mz * _r_mat[2][2];

        // wx = bx * _r_mat[0][0] + bz * _r_mat[2][0];
        // wy = bx * _r_mat[0][1] + bz * _r_mat[2][1];
        // wz = bz * _r_mat[0][2] + bz * _r_mat[2][2];

        wx = bx;
        wy = 0.0f;
        wz = bz;

        TransformEarthToBody(wx, wy, wz);
    
        ex = (ay * _r_mat[2][2] - az * _r_mat[2][1]) + (my * wz - mz * wy);
        ey = (az * _r_mat[2][0] - ax * _r_mat[2][2]) + (mz * wx - mx + wz);
        ez = (ax * _r_mat[2][1] - ay * _r_mat[2][0]) + (mx * wy - my * wx);

        _ex_int += _ki * ex * _dt;
        _ey_int += _ki * ey * _dt;
        _ez_int += _ki * ez * _dt;
    }

#else
    if ((ax != 0.0f) || (ay != 0.0f) || (az != 0.0f))
    {
        normalise = _invSqrt(ax * ax + ay * ay + az * az);
        ax *= normalise;
        ay *= normalise;
        az *= normalise;

        ex = (ay * _r_mat[2][2] - az * _r_mat[2][1]);
        ey = (az * _r_mat[2][0] - ax * _r_mat[2][2]);
        ez = (ax * _r_mat[2][1] - ay * _r_mat[2][0]);

        _ex_int += _ki * ex * _dt;
        _ey_int += _ki * ey * _dt;
        _ez_int += _ki * ez * _dt;

        gx += _kp * ex + _ex_int;
        gy += _kp * ey + _ey_int;
        gz += _kp * ez + _ez_int;
    }
#endif

    q0_last = _q0;
    q1_last = _q1;
    q2_last = _q2;
    q3_last = _q3;
    _q0 += (-q1_last * gx - q2_last * gy - q3_last * gz) * halfT;
    _q1 += (q0_last * gx + q2_last * gz - q3_last * gy) * halfT;
    _q2 += (q0_last * gy - q1_last * gz + q3_last * gx) * halfT;
    _q3 += (q0_last * gz + q1_last * gy - q2_last * gx) * halfT;

    normalise = _invSqrt(_q0 * _q0 + _q1 * _q1 + _q2 * _q2 + _q3 * _q3);
    _q0 *= normalise;
    _q1 *= normalise;
    _q2 *= normalise;
    _q3 *= normalise;

    actual_state.quarter.Set(_q1, _q2, _q3, _q0);
    _computeRotationMatrix();

    actual_state.euler.PITCH = -asinf(_r_mat[2][0]) * RAD2DEG;
    actual_state.euler.ROLL = atan2f(_r_mat[2][1], _r_mat[2][2]) * RAD2DEG;
    actual_state.euler.YAW = atan2f(_r_mat[1][0], _r_mat[0][0]) * RAD2DEG;
}

float Mahony::_invSqrt(float x)
{
    return 1.0f / sqrt(x);
}

void Mahony::_computeRotationMatrix()
{
    float q1q1 = _q1 * _q1;
    float q2q2 = _q2 * _q2;
    float q3q3 = _q3 * _q3;

    float q0q1 = _q0 * _q1;
    float q0q2 = _q0 * _q2;
    float q0q3 = _q0 * _q3;
    float q1q2 = _q1 * _q2;
    float q1q3 = _q1 * _q3;
    float q2q3 = _q2 * _q3;

    _r_mat[0][0] = 1.0f - 2.0f * q2q2 - 2.0f * q3q3;
    _r_mat[0][1] = 2.0f * (q1q2 + -q0q3);
    _r_mat[0][2] = 2.0f * (q1q3 - -q0q2);

    _r_mat[1][0] = 2.0f * (q1q2 - -q0q3);
    _r_mat[1][1] = 1.0f - 2.0f * q1q1 - 2.0f * q3q3;
    _r_mat[1][2] = 2.0f * (q2q3 + -q0q1);

    _r_mat[2][0] = 2.0f * (q1q3 + -q0q2);
    _r_mat[2][1] = 2.0f * (q2q3 - -q0q1);
    _r_mat[2][2] = 1.0f - 2.0f * q1q1 - 2.0f * q2q2;
}

void Mahony::transformBodyToEarth(float &x, float &y, float &z)
{
    float ex = 0.0f;
    float ey = 0.0f;
    float ez = 0.0f;

    ex = x * _r_mat[0][0] + y * _r_mat[0][1] + z * _r_mat[0][2];
    ey = x * _r_mat[1][0] + y * _r_mat[1][1] + z * _r_mat[1][2];
    ez = x * _r_mat[2][0] + y * _r_mat[2][1] + z * _r_mat[2][2];

    x = ex;
    y = ey;
    z = ez;
}

void Mahony::transformEarthToBody(float &x, float &y, float &z)
{
    float bx = 0.0f;
    float by = 0.0f;
    float bz = 0.0f;

    bx = x * _r_mat[0][0] + y * _r_mat[1][0] + z * _r_mat[2][0];
    by = x * _r_mat[0][1] + y * _r_mat[1][1] + z * _r_mat[2][1];
    bz = x * _r_mat[0][2] + y * _r_mat[1][2] + z * _r_mat[2][2];

    x = bx;
    y = by;
    z = bz;
}