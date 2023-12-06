/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-08-07 21:59:47
 * @LastEditors: zhaohe
 * @LastEditTime: 2023-04-09 23:22:33
 * @FilePath: \ZH_FLIGHT\Sys\Sensor\sensor.h
 * Copyright (C) 2022 zhaohe. All rights reserved.
 */
#ifndef SENSOR_H_
#define SENSOR_H_

#include "Sensor/Imu/imu.h"
#include "Sensor/Baro/baro.h"

class SensorData
{
public:
    Vec3 acc;
    Vec3 gyr;
    Vec3 mag;
    float altitude;
};

class Sensor
{
public:
    void init();
    void addImu(Imu *imu);
    void addBaro(Baro *baro);
    JsonTree *createIndex();
    Imu *imu = nullptr;
    Baro *baro = nullptr;
};

#endif
