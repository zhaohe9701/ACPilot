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

#include <math.h>
#include "mahony.h"

#define RAD2DEG 57.29578f
#define DEG2RAD 0.01745329f

void Mahony::update(MemsData &mems, Euler &euler, float dt)
{
    float normalise;

    Vec3 e;
    Vec3 g{mems.gyro.x * DEG2RAD, mems.gyro.y * DEG2RAD, mems.gyro.z * DEG2RAD};
    Vec3 a{mems.acc.x, mems.acc.y, mems.acc.z};
    Vec4 q;

    float halfT = 0.5f * dt;

    if (mems.type() == MEMS_9_AXIS)
    {
        Vec3 m{mems.mag.x, mems.mag.y, mems.mag.z};
        Vec3 h;
        Vec3 b;
        Vec3 w;

        if (((m.x != 0.0f) || (m.y != 0.0f) || (m.z != 0.0f)) && ((a.x != 0.0f) || (a.y != 0.0f) || (a.z != 0.0f)))
        {
            normalise = _invSqrt(a.x * a.x + a.y * a.y + a.z * a.z);
            a.x *= normalise;
            a.y *= normalise;
            a.z *= normalise;

            normalise = _invSqrt(m.x * m.x + m.y * m.y + m.z * m.z);
            m.x *= normalise;
            m.y *= normalise;
            m.z *= normalise;

            h.x = m.x;
            h.y = m.y;
            h.z = m.z;


            transformBodyToEarth(h);

            b.x = sqrt(h.x * h.x + h.y * h.y);
            b.z = h.z;

            // bz = mx * _r_mat[2][0] + my * _r_mat[2][1] + mz * _r_mat[2][2];
            // wx = bx * _r_mat[0][0] + bz * _r_mat[2][0];
            // wy = bx * _r_mat[0][1] + bz * _r_mat[2][1];
            // wz = bz * _r_mat[0][2] + bz * _r_mat[2][2];

            w.x = b.x;
            w.y = 0.0f;
            w.z = b.z;

            transformEarthToBody(w);

            e.x = (a.y * _r_mat[2][2] - a.z * _r_mat[2][1]) + (m.y * w.z - m.z * w.y);
            e.y = (a.z * _r_mat[2][0] - a.x * _r_mat[2][2]) + (m.z * w.x - m.x + w.z);
            e.z = (a.x * _r_mat[2][1] - a.y * _r_mat[2][0]) + (m.x * w.y - m.y * w.x);

            _e_int.x += _ki * e.x * dt;
            _e_int.y += _ki * e.y * dt;
            _e_int.z += _ki * e.z * dt;
        }

    } else
    {
        if ((a.x != 0.0f) || (a.y != 0.0f) || (a.z != 0.0f))
        {
            normalise = _invSqrt(a.x * a.x + a.y * a.y + a.z * a.z);
            a.x *= normalise;
            a.y *= normalise;
            a.z *= normalise;

            e.x = (a.y * _r_mat[2][2] - a.z * _r_mat[2][1]);
            e.y = (a.z * _r_mat[2][0] - a.x * _r_mat[2][2]);
            e.z = (a.x * _r_mat[2][1] - a.y * _r_mat[2][0]);

            _e_int.x += _ki * e.x * dt;
            _e_int.y += _ki * e.y * dt;
            _e_int.z += _ki * e.z * dt;

            g.x += _kp * e.x + _e_int.x;
            g.y += _kp * e.y + _e_int.y;
            g.z += _kp * e.z + _e_int.z;
        }
    }

    q.w = _q.w;
    q.x = _q.x;
    q.y = _q.y;
    q.z = _q.z;
    _q.w += (-q.x * g.x - q.y * g.y - q.z * g.z) * halfT;
    _q.x += (q.w * g.x + q.y * g.z - q.z * g.y) * halfT;
    _q.y += (q.w * g.y - q.x * g.z + q.z * g.x) * halfT;
    _q.z += (q.w * g.z + q.x * g.y - q.y * g.x) * halfT;

    normalise = _invSqrt(_q.w * _q.w + _q.x * _q.x + _q.y * _q.y + _q.z * _q.z);
    _q.w *= normalise;
    _q.x *= normalise;
    _q.y *= normalise;
    _q.z *= normalise;

    _computeRotationMatrix();

    euler.pitch = -asinf(_r_mat[2][0]) * RAD2DEG;
    euler.roll = atan2f(_r_mat[2][1], _r_mat[2][2]) * RAD2DEG;
    euler.yaw = atan2f(_r_mat[1][0], _r_mat[0][0]) * RAD2DEG;
}

void Mahony::reset()
{
    _e_int.clear();
    _q.set(1.0f, 0.0f, 0.0f, 0.0f);
}

float Mahony::_invSqrt(float x)
{
    return 1.0f / sqrt(x);
}

void Mahony::_computeRotationMatrix()
{
    float q1q1 = _q.x * _q.x;
    float q2q2 = _q.y * _q.y;
    float q3q3 = _q.z * _q.z;

    float q0q1 = _q.w * _q.x;
    float q0q2 = _q.w * _q.y;
    float q0q3 = _q.w * _q.z;

    float q1q2 = _q.x * _q.y;
    float q1q3 = _q.x * _q.z;
    float q2q3 = _q.y * _q.z;

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
