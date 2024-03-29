/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-11-13 16:36:18
 * @LastEditors: zhaohe
 * @LastEditTime: 2023-04-09 22:58:52
 * @FilePath: \ZH_FLIGHT\Sys\Sensor\Baro\baro.h
 * Copyright (C) 2022 zhaohe. All rights reserved.
 */
#ifndef BARO_H_
#define BARO_H_

#include <stdint.h>
#include "type.h"

class BaroData
{
public:
    float altitude = 0;
    float pressure = 0;
    float temperature = 0;
};

class Baro
{
public:
    virtual AC_RET init() = 0;
    virtual AC_RET getTemperature(BaroData& data) = 0;
    virtual AC_RET getPressure(BaroData& data) = 0;
    virtual AC_RET getAltitude(BaroData& data) = 0;
protected:
    char _name[PARAM_NAME_LEN] = "baro";
    AcSwitch _get_altitude_mark = AC_OFF;
    AcSwitch _get_pressure_mark = AC_OFF;
    AcSwitch _get_temp_mark = AC_OFF;
    uint8_t _id = 0x00;
};

#endif