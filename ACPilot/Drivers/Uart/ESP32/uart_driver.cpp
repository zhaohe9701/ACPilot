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

#define UART_FREE_DELAY_US 300

using namespace Driver;

UartHandle::UartHandle()
{
    memset(&config, 0, sizeof(uart_config_t));
}

Uart::Uart(UartHandle *handle, uint8_t port_num) : Com(port_num)
{
    _port = port_num;
    this->_handle = handle;
}

AC_RET Uart::init()
{
    char name[PARAM_NAME_LEN] = {0};
    Osal::QueueHandle queue = nullptr;
    if (ESP_OK !=
        uart_driver_install(_handle->index,
                            MAX_UART_BUF_LEN * 2,
                            MAX_UART_BUF_LEN * 2,
                            MAX_UART_BUF_LEN / 8,
                            &queue,
                            0))
    {
        return AC_ERROR;
    }
    _event_queue = new Osal::Queue<uart_event_t>(queue);
    if (ESP_OK != uart_param_config(_handle->index, &_handle->config))
    {
        return AC_ERROR;
    }
    if (ESP_OK != uart_set_pin(_handle->index, _handle->tx, _handle->rx, _handle->rts, _handle->cts))
    {
        return AC_ERROR;
    }

    snprintf(name, PARAM_NAME_LEN, "uart_%d", _handle->index);
    _timer = new HardwareTimer(name, _timer_callback, this);
    _timer->init();
    _uart_task = new Osal::AcThread(name, UART_RECEIVE_TASK_STACK, UART_RECEIVE_TASK_PRIO, UART_RECEIVE_TASK_CORE);
    _uart_task->run(_receive_task, this);
    return AC_OK;
}

AC_RET Uart::send(uint8_t *buf, uint16_t length, uint32_t timeout)
{
    if (length != uart_write_bytes_with_break(_handle->index, buf, length, 10))
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
    if (handle == this->_handle)
    {
        return true;
    } else
    {
        return false;
    }
}

bool Uart::match(uint8_t port_num)
{
    return Com::match(port_num);
}

UartHandle *Uart::getHandle()
{
    return _handle;
}

void Uart::_check()
{

    if (!_timer->isActive())
    {
        _timer->start(UART_FREE_DELAY_US, TIMER_MODE_SINGLE);
    } else
    {
        _timer->stop();
        _timer->start(UART_FREE_DELAY_US, TIMER_MODE_SINGLE);
    }
}

void Uart::_receive_task(void *param)
{
    Uart *uart = static_cast<Uart *>(param);
    uart_event_t event;
    for (;;)
    {
        if (AC_OK == uart->_event_queue->pop(&event, AC_FOREVER))
        {
            switch (event.type)
            {
                case UART_DATA:
                    ((Uart *) param)->_check();
                    break;
                default:
                    break;
            }
        }
    }
}

void Uart::_timer_callback(void *param)
{
    Uart *uart = static_cast<Uart *>(param);
    size_t len = 0;
    uart_get_buffered_data_len(uart->_handle->index, &len);
    uart->_recv_pool = Utils::MemoryPool::getGeneral(len);
    uart->_recv_buffer = uart->_recv_pool->alloc();

    if (nullptr == uart->_recv_buffer)
    {
        uart_flush_input(uart->_handle->index);
        return;
    }

    len = uart_read_bytes(uart->_handle->index,
                          uart->_recv_buffer,
                          uart->_recv_pool->getSize(),
                          AC_IMMEDIATELY);

    if (len == 0 || AC_OK != uart->receive(len))
    {
        uart->_recv_pool->free(uart->_recv_buffer);
        uart->_recv_buffer = nullptr;
        uart->_recv_pool = nullptr;
        return;
    }
}
