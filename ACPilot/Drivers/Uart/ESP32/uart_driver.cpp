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
#include "hal/uart_ll.h"

#define UART_TASK_INTERVAL 1

UartHandle::UartHandle()
{
    memset(&config, 0, sizeof(uart_config_t));
}

Uart::Uart(UartHandle *handle, uint8_t port_num) : ComInterface(port_num)
{
    _port = port_num;
    this->handle = handle;
    UartManager::add(this);
}

AC_RET Uart::init()
{
    if (ESP_OK != uart_driver_install(handle->index, MAX_MESSAGE_BUF_LEN * 2, MAX_MESSAGE_BUF_LEN * 2, 0, nullptr, 0))
    {
        return AC_ERROR;
    }
    if (ESP_OK != uart_param_config(handle->index, &handle->config))
    {
        return AC_ERROR;
    }
    return AC_OK;
}

AC_RET Uart::send(uint8_t *buf, uint16_t length, uint32_t timeout)
{
    if (length != uart_write_bytes_with_break(handle->index, buf, length, 10))
    {
        return AC_ERROR;
    }
    return AC_OK;
}

AC_RET Uart::open()
{
    return AC_OK;
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

void Uart::check()
{
    uint32_t len = 0;
    uart_get_buffered_data_len(handle->index, (size_t*)&len);
    if (len != 0 && tmp_len == len)
    {
        _receive_buffer = _buffer_pool->alloc();
        uart_read_bytes(handle->index, _receive_buffer, MAX_MESSAGE_BUF_LEN, UART_TASK_INTERVAL);
        receive(len);
        tmp_len = 0;
    } else
    {
        tmp_len = len;
    }
}

List<Uart*> UartManager::_list;
bool UartManager::_is_running = false;
AcMutex UartManager::_mutex;
AcThread UartManager::_task{"uart_manager", 256, UART_MANAGER_TASK_PRIO};

void UartManager::add(Uart *uart)
{
    if (!_is_running)
    {
        _list.pushBack(uart);
        _task.run(managerTask, nullptr);
        _is_running = true;
    } else
    {
        _mutex.lock();
        _list.pushBack(uart);
        _mutex.unlock();
    }

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

void UartManager::managerTask(void *pvParameters)
{
    for (;;)
    {
        Tick tick = getTick();
        _mutex.lock();
        for (ListNode<Uart*> *it = UartManager::_list.begin(); it != UartManager::_list.end(); it = it->getNext())
        {
            (**it)->check();
        }
        _mutex.unlock();
        tickSleep(1);
    }
    AcThread::killSelf();
}