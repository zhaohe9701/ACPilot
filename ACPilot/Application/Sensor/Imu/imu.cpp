/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2023-04-10 22:48:12
 * @LastEditors: zhaohe
 * @LastEditTime: 2023-04-11 00:34:12
 * @FilePath: \ZH_FLIGHT\Sys\Sensor\Imu\imu.cpp
 * Copyright (C) 2023 zhaohe. All rights reserved.
 */
#include "imu.h"
#include "sys.h"
#include "Memory/ac_memory.h"

AC_RET Imu::readTmp(TmpData &data)
{
    return AC_NOT_SUPPORT;
}

AC_RET Imu::readGyro(GyroData &data)
{
    return AC_NOT_SUPPORT;
}

AC_RET Imu::readAcc(AccData &data)
{
    return AC_NOT_SUPPORT;
}

AC_RET Imu::readMag(MagData &data)
{
    return AC_NOT_SUPPORT;
}

AC_RET Imu::updateTmp()
{
    return AC_NOT_SUPPORT;
}

AC_RET Imu::updateGyro()
{
    return AC_NOT_SUPPORT;
}

AC_RET Imu::updateAcc()
{
    return AC_NOT_SUPPORT;
}

AC_RET Imu::updateMag()
{
    return AC_NOT_SUPPORT;
}

JsonTree *Imu::CreateIndex()
{
    JsonTree *root = new JsonTree();
    JsonTree *node = nullptr;

    root->addData(nullptr, AC_STRUCT, "IMU");

    node = new JsonTree();
    node->addData((void *) &_name, AC_STRING, "type");
    root->addChild(node);

    node = new JsonTree();
    node->addData((void *) &_id, AC_UINT8, "id");
    root->addChild(node);

    return root;
}

void Imu::_imuWriteRag(uint8_t address, uint8_t value)
{
    _interface->writeReg(address & _WRITE, value);
}

void Imu::_imuReadRag(uint8_t address, uint8_t length, uint8_t *buf)
{
    _interface->readBytes(address | _READ, length, buf);
}

void Imu::_imuWriteRag(uint8_t bank, uint8_t address, uint8_t value)
{
    _interface->writeReg(_REG_BANK_SEL & _WRITE, bank);
    _interface->writeReg(address & _WRITE, value);
    _interface->writeReg(_REG_BANK_SEL & _WRITE, _BANK0);
}

void Imu::_imuReadRag(uint8_t bank, uint8_t address, uint8_t length, uint8_t *buf)
{
    _interface->writeReg(_REG_BANK_SEL & _WRITE, bank);
    _interface->readBytes(address | _READ, length, buf);
    _interface->writeReg(_REG_BANK_SEL & _WRITE, _BANK0);
}




