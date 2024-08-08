//
// Created by zhaohe on 2024/4/13.
//

#include "tdk_imu.h"

using namespace Component;

void TdkImu::_imuWriteRag(uint8_t address, uint8_t value)
{
    _interface->writeReg(address & _WRITE, value, IO_DEFAULT_TIMEOUT);
}

void TdkImu::_imuReadRag(uint8_t address, uint8_t length, uint8_t *buf)
{
    _interface->readBytes(address | _READ, length, buf, IO_DEFAULT_TIMEOUT);
}

void TdkImu::_imuWriteRag(uint8_t bank, uint8_t address, uint8_t value)
{
    _interface->writeReg(_REG_BANK_SEL & _WRITE, bank, IO_DEFAULT_TIMEOUT);
    _interface->writeReg(address & _WRITE, value, IO_DEFAULT_TIMEOUT);
    _interface->writeReg(_REG_BANK_SEL & _WRITE, _BANK0, IO_DEFAULT_TIMEOUT);
}

void TdkImu::_imuReadRag(uint8_t bank, uint8_t address, uint8_t length, uint8_t *buf)
{
    _interface->writeReg(_REG_BANK_SEL & _WRITE, bank, IO_DEFAULT_TIMEOUT);
    _interface->readBytes(address | _READ, length, buf, IO_DEFAULT_TIMEOUT);
    _interface->writeReg(_REG_BANK_SEL & _WRITE, _BANK0, IO_DEFAULT_TIMEOUT);
}
