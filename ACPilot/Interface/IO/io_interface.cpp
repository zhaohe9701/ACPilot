//
// Created by zhaohe on 2024/4/12.
//
#include "io_interface.h"

AC_RET IoInterface::readBytesDMA(uint8_t address, uint8_t len, uint8_t *dataBuf, uint32_t timeout)
{
    return AC_WARN;
}

AC_RET IoInterface::readReg(uint8_t address, uint8_t &value, uint32_t timeout)
{
    return AC_WARN;
}

AC_RET IoInterface::writeBytesDMA(uint8_t address, uint8_t len, uint8_t *dataBuf, uint32_t timeout)
{
    return AC_WARN;
}

AC_RET IoInterface::writeReg(uint8_t address, uint8_t value, uint32_t timeout = 100)
{
    return AC_WARN;
}