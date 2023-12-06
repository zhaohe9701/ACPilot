//
// Created by zhaohe on 2023/8/28.
//
#include "baro.h"
#include "Interface/device_interface.h"

#ifndef DPS310_H_
#define DPS310_H_

class Dps310 : virtual public Baro
{
public:
    explicit Dps310(DeviceInterface *interface);
    AC_RET init() override;
    AC_RET getTemperature(BaroData& data) override;
    AC_RET getPressure(BaroData& data) override;
    AC_RET getAltitude(BaroData& data) override;
    void read();
    ~Dps310() = default;
private:
    DeviceInterface *_interface = nullptr;

    int16_t _c0 = 0, _c1 = 0, _c01 = 0, _c11 = 0, _c20 = 0, _c21 = 0, _c30 = 0;
    int32_t _c00 = 0, _c10 = 0;
    int32_t pressure_scale = 0;
    int32_t temp_scale = 0;
    float _temperature = 0.0f;
    float _pressure = 0.0f;
    void _baroWriteRag(uint8_t address, uint8_t value);
    void _baroReadRag(uint8_t address, uint8_t length, uint8_t *buf);

    AC_RET _readCalibration();

    int32_t _twosComplement(uint32_t val, uint8_t bits);
};
#endif //DPS310_H_
