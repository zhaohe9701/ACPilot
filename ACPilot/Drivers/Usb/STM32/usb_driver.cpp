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
#include "usbd_cdc_if.h"
#include "usb_device.h"

#define MAX_USB_RECEIVE 64

extern "C"
{
    void UsbCallback(uint8_t *buf, uint32_t len)
    {
        UsbManager::usbCallbackHandle(buf, len);
    }
};

Usb::Usb(uint8_t mark) : ComInterface(mark)
{
    UsbManager::add(this);
    MessageTransmitServer::add(this);
}

AC_RET Usb::init()
{
    MX_USB_DEVICE_Init();
    return AC_OK;
}

AC_RET Usb::send(uint8_t* buf, uint16_t length, uint32_t timeout)
{
    for (uint32_t i = 0; i < 10000; ++i)
    {
        if (USBD_OK == CDC_Transmit_FS(buf, length))
        {
            if (nullptr != _transmit_buffer)
            {
                _buffer_pool->free(_transmit_buffer);
            }
            _transmit_buffer = buf;
            return AC_OK;
        }
    }
    _buffer_pool->free(buf);
    return AC_ERROR;
}

AC_RET Usb::open()
{
    _receive_buffer = _buffer_pool->alloc();
    return AC_OK;
}


Usb* UsbManager::_usb = nullptr;

void UsbManager::add(Usb *usb)
{
    _usb = usb;
}

void UsbManager::usbCallbackHandle(uint8_t *buf, uint32_t len)
{
    if (nullptr == _usb || nullptr == _usb->_receive_buffer)
    {
        return;
    }

    if (_usb->_ptr + len > MAX_MESSAGE_BUF_LEN)
    {
        memcpy(_usb->_receive_buffer + _usb->_ptr, buf, MAX_MESSAGE_BUF_LEN - _usb->_ptr - 1);
        len = len - (MAX_MESSAGE_BUF_LEN - _usb->_ptr - 1);
        _usb->_ptr = _usb->_ptr + len;
        _usb->receive(_usb->_ptr);
        memcpy(_usb->_receive_buffer, buf + (MAX_MESSAGE_BUF_LEN - _usb->_ptr - 1), len);
        _usb->_ptr = len;
    }
    memcpy(_usb->_receive_buffer + _usb->_ptr, buf, len);
    _usb->_ptr = _usb->_ptr + len;
    if (len != MAX_USB_RECEIVE || buf[MAX_USB_RECEIVE - 1] == '\n')
    {
        _usb->receive(_usb->_ptr);
        _usb->_ptr = 0;
    }
}


