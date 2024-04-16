/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-10-23 22:51:45
 * @LastEditors: zhaohe
 * @LastEditTime: 2023-03-27 00:15:05
 * @FilePath: \ZH_FLIGHT\Sys\Driver\z_usb.cpp
 * Copyright (C) 2022 zhaohe. All rights reserved.
 */
#include <hal/usb_serial_jtag_ll.h>
#include "Usb/usb_driver.h"
#include "driver/usb_serial_jtag.h"
#include "default_debug.h"

#define USB_FREE_DELAY_US 1000
#define USB_RECEIVE_TASK_PRIO 24
#define USB_RECEIVE_TASK_STACK 2000

Usb::Usb(uint8_t port_num) : Com(port_num)
{
}

AC_RET Usb::init()
{
    usb_serial_jtag_driver_config_t usb_cdc_config = {
            .tx_buffer_size = MAX_USB_BUF_LEN,
            .rx_buffer_size = MAX_USB_BUF_LEN,
    };
    if (ESP_OK != usb_serial_jtag_driver_install(&usb_cdc_config))
    {
        return AC_ERROR;
    }

    _timer = new HardwareTimer("usb", _timer_callback, this);
    _timer->init();
    _usb_task = new AcThread("usb", USB_RECEIVE_TASK_STACK, USB_RECEIVE_TASK_PRIO);
    _usb_task->run(_receive_task, this);
    return AC_OK;
}

AC_RET Usb::send(uint8_t *buf, uint16_t length, uint32_t timeout)
{
    if (nullptr == buf)
    {
        return AC_ERROR;
    }
    usb_serial_jtag_write_bytes(buf, length, AC_FOREVER);
    usb_serial_jtag_ll_txfifo_flush();
    return AC_OK;
}

AC_RET Usb::open()
{
    return AC_OK;
}

#define TMP_BUF_LEN 128

void Usb::_receive_task(void *param)
{
    Usb *usb = static_cast<Usb *>(param);
    char tmp_buf[TMP_BUF_LEN] = {0};
    for (;;)
    {
        uint32_t len = 0;
        if (usb->_current_len >= MAX_USB_BUF_LEN - 1)
        {
            usb->_current_len = 0;
            memset(usb->_buf, 0, MAX_USB_BUF_LEN);
        }

        len = usb_serial_jtag_read_bytes(tmp_buf, TMP_BUF_LEN, AC_FOREVER);

        if (!usb->_timer->isActive())
        {
            memcpy(usb->_buf, tmp_buf, len);
            usb->_current_len = len;
            usb->_timer->start(USB_FREE_DELAY_US, TIMER_MODE_SINGLE);
        } else
        {
            memcpy(usb->_buf + usb->_current_len, tmp_buf, len);
            usb->_current_len += len;
        }
    }
}

void Usb::_timer_callback(void *param)
{
    Usb *usb = static_cast<Usb *>(param);
    uint32_t len = usb->_current_len;

    usb->_recv_pool = MemoryPool::getGeneral(len);
    usb->_recv_buffer = usb->_recv_pool->alloc();
    if (nullptr == usb->_recv_buffer)
    {
        return;
    }

    memcpy(usb->_recv_buffer, usb->_buf, len);
    usb->_recv_buffer[len] = '\0';
    usb->receive(len);
}





