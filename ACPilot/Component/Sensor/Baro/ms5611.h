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

#include "Device/Physical/Baro/baro.h"

#define MS5611_ADDR 0xEE // MS5611的地址

namespace Component
{
    class Ms5611 : public Framework::Baro
    {
    public:
        explicit Ms5611(Interface::IO *interface);

        AC_RET init() override;

        AC_RET getTemp(Framework::TempData &data) override;

        AC_RET getPressure(Framework::AtaData &data) override;

        AC_RET getAltitude(Framework::AltitudeData &data) override;

        AC_RET updateTemp() override;

        AC_RET updatePressure() override;

        AC_RET updateAltitude() override;

        ~Ms5611() = default;

    private:
        uint16_t _calculation[8] = {0};
        int64_t _dt = 0.0f;

        bool _checkCRC();

        void _baroWriteReg(uint8_t address, uint8_t length, uint8_t *value);

        void _baroReadReg(uint8_t address, uint8_t length, uint8_t *buf);
    };
}
#endif
