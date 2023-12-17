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
#include "Usb/usb_driver.h"
#include "driver/usb_serial_jtag.h"

AcThread Usb::_task("usbTask", 512, USB_EVENT_TASK_PRIO);

Usb::Usb(uint8_t port_num): ComInterface(port_num)
{
    UsbManager::add(this);
}

AC_RET Usb::init()
{
    usb_serial_jtag_driver_config_t usb_cdc_config = {
            .tx_buffer_size = MAX_MESSAGE_BUF_LEN,
            .rx_buffer_size = MAX_MESSAGE_BUF_LEN,
    };
    if (ESP_OK != usb_serial_jtag_driver_install(&usb_cdc_config))
    {
        return AC_ERROR;
    }
    return AC_OK;
}

AC_RET Usb::send(uint8_t* buf, uint16_t length, uint32_t timeout)
{
    usb_serial_jtag_write_bytes(buf, length, AC_FOREVER);
    return AC_OK;
}

AC_RET Usb::open()
{
    if (!_isOpen)
    {
        _isOpen = true;
        _task.run(UsbManager::usbReceiveHandle, nullptr);
    }
    return AC_OK;
}

void Usb::test()
{
    while(1)
    {
        printf("test\n");
        tickSleep(1000);
    }

}


Usb* UsbManager::_usb = nullptr;

void UsbManager::add(Usb *usb)
{
    _usb = usb;
}

void UsbManager::usbReceiveHandle(void *param)
{
    for(;;)
    {
        _usb->_receive_buffer = _usb->_buffer_pool->alloc();
        int total_len = 0;
        int len = 0;
        uint8_t *buf = _usb->_receive_buffer;
        do
        {
            len = usb_serial_jtag_read_bytes(buf, MAX_MESSAGE_BUF_LEN - total_len - 1, AC_FOREVER);
            total_len += len;
            buf += len;
        } while (_usb->_receive_buffer[total_len - 1] != '\n' && total_len < MAX_MESSAGE_BUF_LEN - 1);
        _usb->receive(total_len);
    }
    AcThread::killSelf();
}


