//
// Created by zhaohe on 2023/8/28.
//

#include "Device/Physical/Baro/baro.h"

#ifndef DPS310_H_
#define DPS310_H_

class Dps310 : public Baro
{
public:
    explicit Dps310(IoInterface *interface);

    AC_RET init() override;

    AC_RET getTemp(TempData &data) override;

    AC_RET getPressure(AtaData &data) override;

    AC_RET getAltitude(AltitudeData &data) override;

    AC_RET updateTemp() override;

    AC_RET updatePressure() override;

    AC_RET updateAltitude() override;

    ~Dps310() = default;

private:
    IoInterface *_interface = nullptr;

    int16_t _c0 = 0, _c1 = 0, _c01 = 0, _c11 = 0, _c20 = 0, _c21 = 0, _c30 = 0;
    int32_t _c00 = 0, _c10 = 0;
    int32_t _pressure_scale = 0;
    int32_t _temp_scale = 0;
    float _scaled_raw_temp = 0.0f;

    void _baroWriteReg(uint8_t address, uint8_t value);

    void _baroReadReg(uint8_t address, uint8_t length, uint8_t *buf);

    AC_RET _readCalibration();

    int32_t _twosComplement(uint32_t val, uint8_t bits);
};

#endif //DPS310_H_
