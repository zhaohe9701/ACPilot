//
// Created by zhaohe on 2024/4/12.
//
#include "io.h"

using namespace Interface;

AC_RET IO::readBytesDMA(uint8_t address, uint8_t len, uint8_t *dataBuf, uint32_t timeout)
{
    return AC_WARN;
}

AC_RET IO::readReg(uint8_t address, uint8_t &value, uint32_t timeout)
{
    return AC_WARN;
}

AC_RET IO::writeBytesDMA(uint8_t address, uint8_t len, uint8_t *dataBuf, uint32_t timeout)
{
    return AC_WARN;
}

AC_RET IO::writeReg(uint8_t address, uint8_t value, uint32_t timeout = 100)
{
    return AC_WARN;
}