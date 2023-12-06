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
#include "os.h"
#include "config.h"

#define PITCH   x
#define ROLL    y
#define YAW     z

#define MOTOR1 x
#define MOTOR2 y
#define MOTOR3 z
#define MOTOR4 w

typedef float Altitude;

enum Mode
{
    MANUAL_MODE,
    ALTITUDE_MODE,
    AUTO_MODE,
};

enum FlyLock
{
    FLY_LOCK,
    FLY_UNLOCK,
};
class ActualAttitudeState
{
public:
    Vec3 euler;
    Vec3 ang_acc;
    Vec4 quarter;
};

class ExpectAttitudeState
{
public:
    Vec3 euler;
    Vec4 quarter;
};

class AttitudeState
{
public:
    ActualAttitudeState actual;         /* 实际姿态 */
    ExpectAttitudeState expect;         /* 期望姿态 */
};

class ActualPositionState
{
public:
    Vec3 coordinate;                    /* 实际坐标 */
    Vec3 gps;                           /* 实际经纬度 */
    Vec3 baro_altitude;                 /* 气压高度 */
    Vec3 laser_altitude;                /* 激光高度 */
};

class ExpectPositionState
{
public:
    Vec3 coordinate;                    /* 期望坐标 */
    Vec3 gps;                           /* 期望经纬度 */
    float yaw = 0.0f;                   /* 期望yaw */
};

class PositionState
{
public:
    ActualPositionState actual;         /* 实际位置 */
    ExpectPositionState expect;         /* 期望位置 */
};
class UsualState
{
public:
    uint16_t voltage = 0;               /* 电压 */
    uint16_t battery = 0;               /* 电量 */
    FlyLock lock = FLY_UNLOCK;       /* 是否解锁 */
    Mode mode = MANUAL_MODE;         /* 飞行模式 */
    float temperature = 0.0;            /* 温度 */
};


class AircraftState
{
public:
    AttitudeState attitude_state;       /* 姿态 */
    PositionState position_state;       /* 位置 */
    UsualState usual_state;             /* 通用 */
};
#endif

