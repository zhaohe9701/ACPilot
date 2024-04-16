/*
 * @Description:
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-09-27 22:21:16
 * @LastEditors: zhaohe
 * @LastEditTime: 2022-11-13 22:18:10
 * @FilePath: \H7B0\Sys\Driver\z_spi.cpp
 * Copyright (C) 2022 zhaohe. All rights reserved.
 */

#include "Spi/spi_driver.h"
#include "Memory/ac_memory.h"


extern "C"
{
/* SPI发送完成中断处理 */
void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi)
{
    SpiBusManager::writeFinishHandle(hspi);
}
}

extern "C"
{
/* SPI接收完成中断处理 */
void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef *hspi)
{
    SpiBusManager::readFinishHandle(hspi);
}
}

SpiBus::SpiBus(SpiBusHandle *handle)
{
    this->handle = handle;
    SpiBusManager::add(this);
}

AC_RET SpiBus::init() const
{
    if (HAL_SPI_Init(handle) != HAL_OK)
    {
        return AC_ERROR;
    }
    return AC_OK;
}

bool SpiBus::matchHandle(SpiBusHandle *handle) const
{
    if (handle == this->handle)
    {
        return true;
    } else
    {
        return false;
    }
}

AC_RET SpiBus::waitWriteFinish(uint32_t timeout)
{
    return _write_sem.get(timeout);
}

AC_RET SpiBus::waitReadFinish(uint32_t timeout)
{
    return _read_sem.get(timeout);
}

void SpiBus::writeFinishNotify()
{
    _write_sem.give();
}

void SpiBus::readFinishNotify()
{
    _write_sem.give();
}

AC_RET SpiBus::lock(uint32_t timeout)
{
    return _mutex.lock(timeout);
}

void SpiBus::unlock()
{
    _mutex.unlock();
}

List<SpiBus *> SpiBusManager::_list;

void SpiBusManager::add(SpiBus *spi_bus)
{
    _list.pushBack(spi_bus);
}

SpiBus *SpiBusManager::find(SpiBusHandle *handle)
{
    for (ListNode < SpiBus * > *it = _list.begin(); it != _list.end(); it = it->getNext())
    {
        if ((**it)->matchHandle(handle))
        {
            return (**it);
        }
    }
    return nullptr;
}

void SpiBusManager::writeFinishHandle(SpiBusHandle *handle)
{
    SpiBus *bus = find(handle);
    if (nullptr == bus)
    {
        return;
    }
    bus->writeFinishNotify();
}

void SpiBusManager::readFinishHandle(SpiBusHandle *handle)
{
    SpiBus *bus = find(handle);
    if (nullptr == bus)
    {
        return;
    }
    bus->readFinishNotify();
}

Spi::Spi(SpiBus *bus, Gpio *cs)
{
    _bus = bus;
    _cs = cs;
}

AC_RET Spi::readReg(uint8_t address, uint8_t &value, uint32_t timeout)
{
    _bus->lock(timeout);
    _enable();
    HAL_SPI_Transmit(_bus->handle, &address, 1, timeout);
    HAL_SPI_Receive(_bus->handle, &value, 1, timeout);
    _disable();
    _bus->unlock();
    return AC_OK;
}

AC_RET Spi::readBytes(uint8_t address, uint8_t len, uint8_t *dataBuf, uint32_t timeout)
{
    _bus->lock(timeout);
    _enable();
    HAL_SPI_Transmit(_bus->handle, &address, 1, timeout);
    HAL_SPI_Receive(_bus->handle, dataBuf, len, timeout);
    _disable();
    _bus->unlock();
    return AC_OK;
}

AC_RET Spi::readBytesDMA(uint8_t address, uint8_t len, uint8_t *dataBuf, uint32_t timeout)
{
    _bus->lock(timeout);
    _enable();
    HAL_SPI_Transmit(_bus->handle, &address, 1, timeout);
    HAL_SPI_Receive_DMA(_bus->handle, dataBuf, len);
    _bus->waitReadFinish(timeout);
    _disable();
    _bus->unlock();
    return AC_OK;
}

AC_RET Spi::writeReg(uint8_t address, uint8_t value, uint32_t timeout)
{
    _bus->lock(timeout);
    _enable();
    HAL_SPI_Transmit(_bus->handle, &address, 1, timeout);
    HAL_SPI_Transmit(_bus->handle, &value, 1, timeout);
    _disable();
    _bus->unlock();
    return AC_OK;
}

AC_RET Spi::writeBytes(uint8_t address, uint8_t len, uint8_t *value, uint32_t timeout)
{
    _bus->lock(timeout);
    _enable();
    HAL_SPI_Transmit(_bus->handle, &address, 1, timeout);
    HAL_SPI_Transmit(_bus->handle, value, len, timeout);
    _disable();
    _bus->unlock();
    return AC_OK;
}

AC_RET Spi::writeBytesDMA(uint8_t address, uint8_t len, uint8_t *value, uint32_t timeout)
{
    _bus->lock(timeout);
    _enable();
    HAL_SPI_Transmit(_bus->handle, &address, 1, timeout);
    HAL_SPI_Transmit_DMA(_bus->handle, value, len);
    _bus->waitWirteFinish(timeout);
    _disable();
    _bus->unlock();
    return AC_OK;
}

void Spi::_enable()
{
    _cs->toReset();
}

void Spi::_disable()
{
    _cs->toSet();
}
