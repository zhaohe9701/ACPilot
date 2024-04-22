/*
 * @Description:
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-09-27 22:23:58
 * @LastEditors: zhaohe
 * @LastEditTime: 2023-01-26 01:00:42
 * @FilePath: \ZH_FLIGHT\Sys\Driver\z_iic.cpp
 * Copyright (C) 2022 zhaohe. All rights reserved.
 */
#include "Iic/iic_driver.h"

IicBusHandle::IicBusHandle()
{
    memset(&config, 0, sizeof(i2c_config_t));
}

IicBus::IicBus(IicBusHandle *handle)
{
    this->handle = handle;
}

AC_RET IicBus::init() const
{
    if (ESP_OK != i2c_param_config(handle->host, &handle->config))
    {
        return AC_ERROR;
    }
    if (ESP_OK != i2c_driver_install(handle->host, handle->config.mode, 0, 0, 0))
    {
        return AC_ERROR;
    }
    return AC_OK;
}

bool IicBus::matchHandle(IicBusHandle *handle) const
{
    if (handle == this->handle)
    {
        return true;
    } else
    {
        return false;
    }
}

AC_RET IicBus::lock(uint32_t timeout)
{
    return _mutex.lock(timeout);
}

void IicBus::unlock()
{
    _mutex.unlock();
}

Iic::Iic(IicBus *bus, IicHandle *handle)
{
    _bus = bus;
    _handle = handle;
}

AC_RET Iic::init()
{
    return AC_OK;
}

AC_RET Iic::readReg(uint8_t address, uint8_t &value, uint32_t timeout)
{
    _bus->lock(timeout);
    i2c_master_write_read_device(_bus->handle->host, _handle->addr, &address, 1, &value, 1, timeout);
    _bus->unlock();
    return AC_OK;
}

AC_RET Iic::readBytes(uint8_t address, uint8_t len, uint8_t *dataBuf, uint32_t timeout)
{
    _bus->lock(timeout);
    i2c_master_write_read_device(_bus->handle->host, _handle->addr, &address, 1, dataBuf, len, timeout);
    _bus->unlock();
    return AC_OK;
}

AC_RET Iic::readBytesDMA(uint8_t address, uint8_t len, uint8_t *dataBuf, uint32_t timeout)
{
    return AC_NOT_SUPPORT;
}

AC_RET Iic::writeReg(uint8_t address, uint8_t value, uint32_t timeout)
{
    uint8_t buf[2] = {address, value};
    _bus->lock(timeout);
    i2c_master_write_to_device(_bus->handle->host, _handle->addr, buf, 2, timeout);
    _bus->unlock();
    return AC_OK;
}

AC_RET Iic::writeBytes(uint8_t address, uint8_t len, uint8_t *value, uint32_t timeout)
{
    return AC_NOT_SUPPORT;
}

AC_RET Iic::writeBytesDMA(uint8_t address, uint8_t len, uint8_t *value, uint32_t timeout)
{
    return AC_NOT_SUPPORT;
}
