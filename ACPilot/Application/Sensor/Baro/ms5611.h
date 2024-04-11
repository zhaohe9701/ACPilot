/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-11-13 19:36:40
 * @LastEditors: zhaohe
 * @LastEditTime: 2023-04-09 22:59:11
 * @FilePath: \ZH_FLIGHT\Sys\Sensor\Baro\ms5611.h
 * Copyright (C) 2022 zhaohe. All rights reserved.
 */
#ifndef MS5611_H_
#define MS5611_H_

#include "IO/device_interface.h"
#include "Sensor/Baro/baro.h"

#define MS5611_ADDR 0xEE // MS5611的地址
class Ms5611 : virtual public Baro
{
public:
    explicit Ms5611(DeviceInterface *interface);
    AC_RET init() override;
    AC_RET getTemperature(BaroData& data) override;
    AC_RET getPressure(BaroData& data) override;
    AC_RET getAltitude(BaroData& data) override;
    ~Ms5611() = default;
private:
    DeviceInterface *_interface;
    uint16_t _calculation[8] = {0};
    int64_t _dt = 0.0f;
    int64_t _temperature = 0.0f;
    int64_t _pressure = 0.0f;
    bool _checkCRC();
    void _baroWriteRag(uint8_t address, uint8_t length, uint8_t *value);
    void _baroReadRag(uint8_t address, uint8_t length, uint8_t *buf);
};

#endif
