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

extern "C"
{
void HAL_I2C_MemRxCpltCallback(I2C_HandleTypeDef *hi2c)
{
    IicBusManager::readFinishHandle(hi2c);
}
}

extern "C"
{
void HAL_I2C_MemTxCpltCallback(I2C_HandleTypeDef *hi2c)
{
    IicBusManager::writeFinishHandle(hi2c);
}
}

extern "C"
{
void HAL_I2C_MasterTxCpltCallback(I2C_HandleTypeDef *hi2c)
{
    IicBusManager::writeFinishHandle(hi2c);
}
}


IicBus::IicBus()
{
    handle = new IicBusHandle();
    IicBusManager::add(this);
}

IicBus::IicBus(IicBusHandle *handle)
{
    this->handle = handle;
    IicBusManager::add(this);
}

IicBus::IicBus(Gpio *sda, Gpio *scl)
{
    handle = new IicBusHandle();
    _sda = sda;
    _scl = scl;
    IicBusManager::add(this);
}

AC_RET IicBus::init(IicBusSelect *iic_name, uint32_t frq)
{
    handle->Instance = iic_name;
    handle->Init.Timing = frq;
    handle->Init.OwnAddress1 = 0;
    handle->Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
    handle->Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
    handle->Init.OwnAddress2 = 0;
    handle->Init.OwnAddress2Masks = I2C_OA2_NOMASK;
    handle->Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
    handle->Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
    if (HAL_I2C_Init(handle) != HAL_OK)
    {
        return AC_ERROR;
    }
    if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
    {
        return AC_ERROR;
    }
    if (HAL_I2CEx_ConfigDigitalFilter(handle, 0) != HAL_OK)
    {
        return AC_ERROR;
    }
    return AC_OK;
}

AC_RET IicBus::initAdvanced()
{
    if (HAL_I2C_Init(handle) != HAL_OK)
    {
        return AC_ERROR;
    }
    if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
    {
        return AC_ERROR;
    }
    if (HAL_I2CEx_ConfigDigitalFilter(handle, 0) != HAL_OK)
    {
        return AC_ERROR;
    }
    return AC_OK;
}

bool IicBus::matchHandle(IicBusHandle *hdle)
{
    if (hdle == this->handle)
    {
        return true;
    } else
    {
        return false;
    }
}

AC_RET IicBus::waitWirteFinish(uint32_t timeout)
{
    return _write_sem.get(timeout);
}

AC_RET IicBus::waitReadFinish(uint32_t timeout)
{
    return _read_sem.get(timeout);
}

void IicBus::writeFinishNotify()
{
    _write_sem.give();
}

void IicBus::readFinishNotify()
{
    _write_sem.give();
}

AC_RET IicBus::lock(uint32_t timeout)
{
    return _mutex.lock(timeout);
}

void IicBus::unlock()
{
    _mutex.unlock();
}

List<IicBus *> IicBusManager::_list;

void IicBusManager::add(IicBus *Iic_bus)
{
    _list.pushBack(Iic_bus);
}

IicBus *IicBusManager::find(IicBusHandle *handle)
{
    for (ListNode < IicBus * > *it = _list.begin(); it != _list.end(); it = it->getNext())
    {
        if ((**it)->matchHandle(handle))
        {
            return (**it);
        }
    }
    return nullptr;
}

void IicBusManager::writeFinishHandle(IicBusHandle *handle)
{
    IicBus *bus = find(handle);
    if (nullptr == bus)
    {
        return;
    }
    bus->writeFinishNotify();
}

void IicBusManager::readFinishHandle(IicBusHandle *handle)
{
    IicBus *bus = find(handle);
    if (nullptr == bus)
    {
        return;
    }
    bus->readFinishNotify();
}

Iic::Iic(IicBus *bus, uint16_t addr)
{
    _bus = bus;
    _device_addr = addr;
}

AC_RET Iic::readReg(uint8_t address, uint8_t &value, uint32_t timeout)
{
    _bus->lock(timeout);
    HAL_I2C_Mem_Read(_bus->handle, _device_addr, address, I2C_MEMADD_SIZE_8BIT, &value, 1, timeout);
    _bus->unlock();
    return AC_OK;
}

AC_RET Iic::readBytes(uint8_t address, uint8_t len, uint8_t *dataBuf, uint32_t timeout)
{
    _bus->lock(timeout);
    HAL_I2C_Mem_Read(_bus->handle, _device_addr, address, I2C_MEMADD_SIZE_8BIT, dataBuf, len, timeout);
    _bus->unlock();
    return AC_OK;
}

AC_RET Iic::readBytesDMA(uint8_t address, uint8_t len, uint8_t *dataBuf, uint32_t timeout)
{
    _bus->lock(timeout);
    HAL_I2C_Mem_Read_DMA(_bus->handle, _device_addr, address, I2C_MEMADD_SIZE_8BIT, dataBuf, len);
    _bus->waitReadFinish(timeout);
    _bus->unlock();
    return AC_OK;
}

AC_RET Iic::writeReg(uint8_t address, uint8_t value, uint32_t timeout)
{
    _bus->lock(timeout);
    HAL_I2C_Mem_Write(_bus->handle, _device_addr, address, I2C_MEMADD_SIZE_8BIT, &value, 1, timeout);
    _bus->unlock();
    return AC_OK;
}

AC_RET Iic::writeBytes(uint8_t address, uint8_t len, uint8_t *value, uint32_t timeout)
{
    _bus->lock(timeout);
    if (0 == len)
    {
        HAL_I2C_Master_Transmit(_bus->handle, _device_addr, &address, 1, timeout);
    } else
    {
        HAL_I2C_Mem_Write(_bus->handle, _device_addr, address, I2C_MEMADD_SIZE_8BIT, value, len, timeout);
    }
    _bus->unlock();
    return AC_OK;
}

AC_RET Iic::writeBytesDMA(uint8_t address, uint8_t len, uint8_t *value, uint32_t timeout)
{
    _bus->lock(timeout);
    if (0 == len)
    {
        HAL_I2C_Master_Transmit_DMA(_bus->handle, _device_addr, &address, 1);
    } else
    {
        HAL_I2C_Mem_Write_DMA(_bus->handle, _device_addr, address, I2C_MEMADD_SIZE_8BIT, value, len);
    }
    _bus->waitWirteFinish(timeout);
    _bus->unlock();
    return AC_OK;
}
