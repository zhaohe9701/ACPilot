/*
 * @brief 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-10-22 00:00:49
 * @LastEditors: zhaohe
 * @LastEditTime: 2023-01-28 03:21:08
 * @FilePath: \ZH_FLIGHT\Sys\Driver\z_uart.cpp
 * Copyright (C) 2022 zhaohe. All rights reserved.
 */
#include "Uart/uart_driver.h"
#include "config.h"
#include "type.h"
#include "ac_list.h"
#include "Memory/ac_memory.h"
extern "C"
{
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
    UartManager::transmitFinishHandle(huart);
}
}

extern "C"
{
void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
    UartManager::uartRestartHandle(huart);
}
}

extern "C"
{
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
    UartManager::receiveFinishHandle(huart, Size);
}
}

Uart::Uart(UartHandle *handle, uint8_t port_num) : ComInterface(port_num)
{
    _port = port_num;
    this->handle = handle;
    UartManager::add(this);
}

AC_RET Uart::init()
{
    if (HAL_UART_Init(handle) != HAL_OK)
    {
        return AC_ERROR;
    }
    if (HAL_UARTEx_SetTxFifoThreshold(handle, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
    {
        return AC_ERROR;
    }
    if (HAL_UARTEx_SetRxFifoThreshold(handle, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
    {
        return AC_ERROR;
    }
    if (HAL_UARTEx_DisableFifoMode(handle) != HAL_OK)
    {
        return AC_ERROR;
    }
    return AC_OK;
}

AC_RET Uart::send(uint8_t *buf, uint16_t length, uint32_t timeout)
{
    waitSendFinish(timeout);
    _transmit_buffer = buf;
    if(HAL_OK == HAL_UART_Transmit_DMA(handle, _transmit_buffer, length))
    {
        return AC_OK;
    }
    else
    {
        return AC_ERROR;
    }
}

AC_RET Uart::open()
{
    _receive_buffer = _buffer_pool->alloc();

    if (nullptr == _receive_buffer)
    {
        return AC_ERROR;
    }

    if (HAL_OK != HAL_UARTEx_ReceiveToIdle_DMA(handle, _receive_buffer, MAX_MESSAGE_BUF_LEN))
    {
        return AC_ERROR;
    }

    return AC_OK;
}

void Uart::sendFinish()
{
    _buffer_pool->free(_transmit_buffer);
    _transmit_buffer = nullptr;
    _sem.give();
}

void Uart::waitSendFinish(uint32_t timeout)
{
    _sem.get(timeout);
}

/**
 * @brief 
 * @param {UART_HandleTypeDef} *handle
 * @return {*}
 */
bool Uart::match(UartHandle *handle)
{
    if (handle == this->handle)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool Uart::match(uint8_t port_num)
{
    return ComInterface::match(port_num);
}

UartHandle *Uart::getHandle()
{
    return handle;
}

List<Uart*> UartManager::_list;

void UartManager::add(Uart *uart)
{
    _list.pushBack(uart);
}

Uart* UartManager::find(UartHandle *handle)
{
    for (ListNode<Uart*> *it = _list.begin(); it != _list.end(); it = it->getNext())
    {
        if ((**it)->match(handle))
        {
            return (**it);
        }
    }
    return nullptr;
}

Uart* UartManager::find(uint8_t port_num)
{
    for (ListNode<Uart*> *it = _list.begin(); it != _list.end(); it = it->getNext())
    {
        if ((**it)->match(port_num))
        {
            return (**it);
        }
    }
    return nullptr;
}


void UartManager::receiveFinishHandle(UartHandle *handle, uint16_t size)
{
    Uart *uart = nullptr;
    uart = UartManager::find(handle);
    if (nullptr == uart)
    {
        return;
    }
    uart->receive(size);
}

void UartManager::uartRestartHandle(UartHandle *handle)
{
    Uart *uart = nullptr;

    uart = UartManager::find(handle);
    if (nullptr == uart)
    {
        return;
    }

    for (int i = 0; i < 3; ++i)
    {
        uart->_buffer_pool->free(uart->_receive_buffer);
        if (AC_OK == uart->open())
        {
            break;
        }
    }
}

void UartManager::transmitFinishHandle(UartHandle *handle)
{
    Uart *uart = nullptr;

    uart = UartManager::find(handle);
    if (nullptr == uart)
    {
        return;
    }
    uart->sendFinish();
}
