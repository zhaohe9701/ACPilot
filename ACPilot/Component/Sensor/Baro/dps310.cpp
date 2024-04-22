//
// Created by zhaohe on 2023/8/28.
//
#include <math.h>
#include <string.h>
#include "Dps310.h"
#include "os.h"

#define READ                0x80
#define WRITE               0x7f

#define DPS310_PRSB2        0x00    ///< Highest byte of pressure data
#define DPS310_TMPB2        0x03    ///< Highest byte of temperature data
#define DPS310_PRSCFG       0x06    ///< Pressure configuration
#define DPS310_TMPCFG       0x07    ///< Temperature configuration
#define DPS310_MEASCFG      0x08    ///< Sensor configuration
#define DPS310_CFGREG       0x09    ///< Interrupt/FIFO configuration
#define DPS310_RESET        0x0C    ///< Soft reset
#define DPS310_PRODREVID    0x0D    ///< Register that contains the part ID
#define DPS310_TMPCOEFSRCE  0x28    ///< Temperature calibration src

/* 524288, 1572864, 3670016, 7864320, 253952, 516096, 1040384, 2088960 */

Dps310::Dps310(IoInterface *interface)
{
    _interface = interface;
    strncpy(_name, "DPS310", sizeof(_name));
    _ability = (1U << BAROMETER_DEV) |
               (1U << ALTIMETER_DEV) |
               (1U << THERMOMETER_DEV);
}

AC_RET Dps310::init()
{
    uint8_t cfg = 0x0;
    _baroReadReg(DPS310_PRODREVID, 1, &_id);
    if (0x10 != _id)
    {
        BASE_ERROR("DPS310: ID error");
        return AC_ERROR;
    }
    tickSleep(10);
    _readCalibration();
    tickSleep(10);
    _baroWriteReg(DPS310_RESET, 0b10001001);
    tickSleep(10);
    _baroWriteReg(DPS310_PRSCFG, 0b01000100);
    tickSleep(10);
    _baroReadReg(DPS310_TMPCOEFSRCE, 1, &cfg);
    cfg = cfg & 0x80;
    tickSleep(10);
    _baroWriteReg(DPS310_TMPCFG, 0b01000100 + cfg);
    tickSleep(10);
    _baroWriteReg(DPS310_MEASCFG, 0b00000111);
    tickSleep(10);
    _baroWriteReg(DPS310_CFGREG, 0b00001100);
    _pressure_scale = 253952;
    _temp_scale = 253952;
    return AC_OK;
}

AC_RET Dps310::getTemp(TempData &data)
{
    data = _temperature_data;
    return AC_OK;
}

AC_RET Dps310::getPressure(AtaData &data)
{
    data = _pressure_data;
    return AC_OK;
}

AC_RET Dps310::getAltitude(AltitudeData &data)
{
    data = _altitude_data;
    return AC_OK;
}

AC_RET Dps310::_readCalibration()
{
    uint8_t cfg = 0x0;
    uint8_t co_effs[18] = {0};

    do
    {
        tickSleep(10);
        _baroReadReg(DPS310_MEASCFG, 1, &cfg);
    } while (!(cfg & (0x01 << 7)));
    _baroReadReg(0x10, 18, co_effs);

    _c0 = (int16_t) (((uint16_t) co_effs[0] << 4) | (((uint16_t) co_effs[1] >> 4) & 0x0F));
    _c0 = (int16_t) _twosComplement(_c0, 12);

    _c1 = (int16_t) _twosComplement((((uint16_t) co_effs[1] & 0x0F) << 8) | co_effs[2], 12);

    _c00 = (int32_t) (((uint32_t) co_effs[3] << 12) | ((uint32_t) co_effs[4] << 4) |
                      (((uint32_t) co_effs[5] >> 4) & 0x0F));
    _c00 = _twosComplement(_c00, 20);

    _c10 = (int32_t) ((((uint32_t) co_effs[5] & 0x0F) << 16) | ((uint32_t) co_effs[6] << 8) | (uint32_t) co_effs[7]);
    _c10 = _twosComplement(_c10, 20);

    _c01 = (int16_t) _twosComplement(((uint16_t) co_effs[8] << 8) | (uint16_t) co_effs[9], 16);
    _c11 = (int16_t) _twosComplement(((uint16_t) co_effs[10] << 8) | (uint16_t) co_effs[11], 16);
    _c20 = (int16_t) _twosComplement(((uint16_t) co_effs[12] << 8) | (uint16_t) co_effs[13], 16);
    _c21 = (int16_t) _twosComplement(((uint16_t) co_effs[14] << 8) | (uint16_t) co_effs[15], 16);
    _c30 = (int16_t) _twosComplement(((uint16_t) co_effs[16] << 8) | (uint16_t) co_effs[17], 16);

    return AC_OK;
}

int32_t Dps310::_twosComplement(uint32_t val, uint8_t bits)
{
    int32_t max_signed_value_exclusive = 0;
    int32_t complement = 0;

    max_signed_value_exclusive = (int32_t) ((uint32_t) 1U << (bits - 1U));
    complement = (int32_t) val;
    if (complement >= max_signed_value_exclusive)
    {
        complement -= 2 * max_signed_value_exclusive;
    }
    return complement;
}

void Dps310::_baroWriteReg(uint8_t address, uint8_t value)
{
    _interface->writeReg(address & WRITE, value, IO_DEFAULT_TIMEOUT);
}

void Dps310::_baroReadReg(uint8_t address, uint8_t length, uint8_t *buf)
{
    _interface->readBytes(address | READ, length, buf, IO_DEFAULT_TIMEOUT);
}

AC_RET Dps310::updateTemp()
{
    int32_t raw_temperature = 0;
    uint8_t raw_data[3] = {0};

    _baroReadReg(DPS310_TMPB2, 3, raw_data);

    raw_temperature = _twosComplement(
            (uint32_t) raw_data[0] << 16 | (uint32_t) raw_data[1] << 8 | (uint32_t) raw_data[2], 24);
    _scaled_raw_temp = (float) raw_temperature / (float) _temp_scale;
    _temperature_data.x = _scaled_raw_temp * (float) _c1 + (float) _c0 / 2.0f;
    return AC_OK;
}

AC_RET Dps310::updatePressure()
{
    int32_t raw_pressure = 0;
    uint8_t raw_data[3] = {0};
    _baroReadReg(DPS310_PRSB2, 3, raw_data);
    raw_pressure = _twosComplement((uint32_t) raw_data[0] << 16 | (uint32_t) raw_data[1] << 8 | (uint32_t) raw_data[2],
                                   24);
    _pressure_data.x = (float) raw_pressure / (float) _pressure_scale;

    _pressure_data.x =
            (float) _c00 +
            _pressure_data.x * ((float) _c10 +
                                _pressure_data.x * ((float) _c20 + _pressure_data.x * (float) _c30)) +
            _scaled_raw_temp *
            ((float) _c01 +
             _pressure_data.x * ((float) _c11 + _pressure_data.x * (float) _c21));

    return AC_OK;
}

AC_RET Dps310::updateAltitude()
{
    updateTemp();
    updatePressure();
    float sea_level_hpa = 1013.25f;  // standard atmospheric pressure
    _altitude_data.x = 44330.0f * (1.0f - pow((_pressure_data.x / 100.0f) / sea_level_hpa, 0.1903f));

    return AC_OK;
}
