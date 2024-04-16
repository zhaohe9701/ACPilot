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

#include "Device/device_data.h"
#include "IO/io_interface.h"
#include "Device/Physical/physical_device.h"

class Baro : public PhysicalDevice
{
public:
    Baro() = default;

    virtual AC_RET init();

    virtual AC_RET getTemp(TempData &data);

    virtual AC_RET getPressure(AtaData &data);

    virtual AC_RET getAltitude(AltitudeData &data);

    virtual AC_RET updateTemp();

    virtual AC_RET updatePressure();

    virtual AC_RET updateAltitude();

    AC_RET readAltimeter(AltitudeData &data) override;

    AC_RET readThermometer(TempData &data) override;

    AC_RET readBarometer(AtaData &data) override;

protected:
    uint8_t _id = 0x00;
    IoInterface *_interface = nullptr;
    char _name[PARAM_NAME_LEN] = "baro";

    AltitudeData _altitude_data{0.0f};
    TempData _temperature_data{0.0f};
    AtaData _pressure_data{0.0f};
};

#endif