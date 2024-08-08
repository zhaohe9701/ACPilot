/*
 * @Description:
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-07-07 23:44:27
 * @LastEditors: zhaohe
 * @LastEditTime: 2023-02-25 02:05:47
 * @FilePath: \ZH_FLIGHT\Sys\Sensor\sensor_interface.h
 * Copyright (C) 2022 zhaohe. All rights reserved.
 */
#ifndef IO_H_
#define IO_H_

#include "Type/type.h"

#define IO_DEFAULT_TIMEOUT 100

namespace Interface
{
    class IO
    {
    public:
        IO() = default;

        virtual AC_RET readBytes(uint8_t address, uint8_t len, uint8_t *dataBuf, uint32_t timeout) = 0;

        virtual AC_RET readBytesDMA(uint8_t address, uint8_t len, uint8_t *dataBuf, uint32_t timeout);

        virtual AC_RET readReg(uint8_t address, uint8_t &value, uint32_t timeout);

        virtual AC_RET writeBytes(uint8_t address, uint8_t len, uint8_t *value, uint32_t timeout) = 0;

        virtual AC_RET writeBytesDMA(uint8_t address, uint8_t len, uint8_t *dataBuf, uint32_t timeout);

        virtual AC_RET writeReg(uint8_t address, uint8_t value, uint32_t timeout);

        virtual ~IO() = default;
    };
}

#endif