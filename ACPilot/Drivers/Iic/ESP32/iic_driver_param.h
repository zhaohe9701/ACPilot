/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-09-27 22:23:50
 * @LastEditors: zhaohe
 * @LastEditTime: 2023-02-25 02:06:12
 * @FilePath: \ZH_FLIGHT\Sys\Driver\z_iic.h
 * Copyright (C) 2022 zhaohe. All rights reserved.
 */
#ifndef IIC_DRIVER_PARAM_H_
#define IIC_DRIVER_PARAM_H_

#include "driver/i2c.h"
#include "string.h"

struct IicBusHandle
{
    IicBusHandle();
    i2c_config_t config{};
    i2c_port_t host = I2C_NUM_0;
};

IicBusHandle::IicBusHandle()
{
    memset(&config, 0, sizeof(i2c_config_t));
}

struct IicHandle
{
    uint8_t addr = 0x0;
};

#endif