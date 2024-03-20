//
// Created by zhaohe on 2023/8/28.
//
#include "Dps310.h"
#include "os.h"
#include <math.h>

#define READ 0x80
#define WRITE 0x7f

#define DPS310_PRSB2 0x00       ///< Highest byte of pressure data
#define DPS310_TMPB2 0x03       ///< Highest byte of temperature data
#define DPS310_PRSCFG 0x06      ///< Pressure configuration
#define DPS310_TMPCFG 0x07      ///< Temperature configuration
#define DPS310_MEASCFG 0x08     ///< Sensor configuration
#define DPS310_CFGREG 0x09      ///< Interrupt/FIFO configuration
#define DPS310_RESET 0x0C       ///< Soft reset
#define DPS310_PRODREVID 0x0D   ///< Register that contains the part ID
#define DPS310_TMPCOEFSRCE 0x28 ///< Temperature calibration src

/* 524288, 1572864, 3670016, 7864320, 253952, 516096, 1040384, 2088960 */

Dps310::Dps310(DeviceInterface *interface)
{
    _interface = interface;
}

AC_RET Dps310::init()
{
    uint8_t cfg = 0x0;
    _baroReadRag(DPS310_PRODREVID, 1, &_id);
    if (0x10 != _id)
    {
        return AC_ERROR;
    }
    tickSleep(10);
    _readCalibration();
    tickSleep(10);
    _baroWriteRag(DPS310_RESET, 0b10001001);
    tickSleep(10);
    _baroWriteRag(DPS310_PRSCFG, 0b01000100);
    tickSleep(10);
    _baroReadRag(DPS310_TMPCOEFSRCE, 1, &cfg);
    cfg = cfg & 0x80;
    tickSleep(10);
    _baroWriteRag(DPS310_TMPCFG, 0b01000100 + cfg);
    tickSleep(10);
    _baroWriteRag(DPS310_MEASCFG, 0b00000111);
    tickSleep(10);
    _baroWriteRag(DPS310_CFGREG, 0b00001100);
    pressure_scale = 253952;
    temp_scale = 253952;
    return AC_OK;
}

AC_RET Dps310::getTemperature(BaroData &data)
{
    data.temperature = _temperature;
    return AC_OK;
}

AC_RET Dps310::getPressure(BaroData &data)
{
    data.pressure = _pressure;
    return AC_OK;
}

AC_RET Dps310::getAltitude(BaroData &data)
{
    float seaLevelhPa = 1013.25f;  // standard atmospheric pressure
    data.altitude = 44330.0f * (1.0f - pow((_pressure / 100.0f) / seaLevelhPa, 0.1903f));
    return AC_OK;
}

AC_RET Dps310::_readCalibration()
{
    uint8_t cfg = 0x0;
    uint8_t coeffs[18] = {0};

    do
    {
        tickSleep(10);
        _baroReadRag(DPS310_MEASCFG, 1, &cfg);
    } while (!(cfg & (0x01 << 7)));
    _baroReadRag(0x10, 18, coeffs);

    _c0 = (int16_t) (((uint16_t) coeffs[0] << 4) | (((uint16_t) coeffs[1] >> 4) & 0x0F));
    _c0 = (int16_t) _twosComplement(_c0, 12);

    _c1 = (int16_t) _twosComplement((((uint16_t) coeffs[1] & 0x0F) << 8) | coeffs[2], 12);

    _c00 = (int32_t) (((uint32_t) coeffs[3] << 12) | ((uint32_t) coeffs[4] << 4) |
                      (((uint32_t) coeffs[5] >> 4) & 0x0F));
    _c00 = _twosComplement(_c00, 20);

    _c10 = (int32_t) ((((uint32_t) coeffs[5] & 0x0F) << 16) | ((uint32_t) coeffs[6] << 8) | (uint32_t) coeffs[7]);
    _c10 = _twosComplement(_c10, 20);

    _c01 = (int16_t) _twosComplement(((uint16_t) coeffs[8] << 8) | (uint16_t) coeffs[9], 16);
    _c11 = (int16_t) _twosComplement(((uint16_t) coeffs[10] << 8) | (uint16_t) coeffs[11], 16);
    _c20 = (int16_t) _twosComplement(((uint16_t) coeffs[12] << 8) | (uint16_t) coeffs[13], 16);
    _c21 = (int16_t) _twosComplement(((uint16_t) coeffs[14] << 8) | (uint16_t) coeffs[15], 16);
    _c30 = (int16_t) _twosComplement(((uint16_t) coeffs[16] << 8) | (uint16_t) coeffs[17], 16);

    return AC_OK;
}

void Dps310::read()
{
    int32_t raw_pressure = 0;
    int32_t raw_temperature = 0;
    uint8_t raw_data[3] = {0};
    float scaled_rawtemp = 0.0f;
    _baroReadRag(DPS310_PRSB2, 3, raw_data);
    raw_pressure = _twosComplement((uint32_t) raw_data[0] << 16 | (uint32_t) raw_data[1] << 8 | (uint32_t) raw_data[2],
                                   24);
    _baroReadRag(DPS310_TMPB2, 3, raw_data);
    raw_temperature = _twosComplement(
            (uint32_t) raw_data[0] << 16 | (uint32_t) raw_data[1] << 8 | (uint32_t) raw_data[2], 24);
    scaled_rawtemp = (float) raw_temperature / (float) temp_scale;
    _temperature = scaled_rawtemp * (float) _c1 + (float) _c0 / 2.0f;

    _pressure = (float) raw_pressure / (float) pressure_scale;

    _pressure =
            (float) _c00 +
            _pressure * ((float) _c10 +
                         _pressure * ((float) _c20 + _pressure * (float) _c30)) +
            scaled_rawtemp *
            ((float) _c01 +
             _pressure * ((float) _c11 + _pressure * (float) _c21));
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

void Dps310::_baroWriteRag(uint8_t address, uint8_t value)
{
    _interface->writeReg(address & WRITE, value);
}

void Dps310::_baroReadRag(uint8_t address, uint8_t length, uint8_t *buf)
{
    _interface->readBytes(address | READ, length, buf);
}
