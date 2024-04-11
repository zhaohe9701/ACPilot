/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2023-04-09 13:56:39
 * @LastEditors: zhaohe
 * @LastEditTime: 2023-04-09 15:15:02
 * @FilePath: \ZH_FLIGHT\Sys\Sensor\Mag\ak09916.h
 * Copyright (C) 2023 zhaohe. All rights reserved.
 */
#ifndef AK09916_H_
#define AK09916_H_

#include "IO/device_interface.h"
#include "Device/magnetometer.h"

class Ak09916
{
public:
    void init();
    uint8_t getId();
    void getMagData(MagData &data);
private:
    uint8_t _last_bank = 0xFF;
    DeviceInterface *_interface = nullptr;
    void _masterWriteRag(uint8_t bank, uint8_t address, uint8_t value);
    void _masterReadRag(uint8_t bank, uint8_t address, uint8_t length, uint8_t *buf);
    void _loadIicRegParam();

};
#endif