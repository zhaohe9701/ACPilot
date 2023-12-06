//
// Created by zhaohe on 2023/8/19.
//
#include "com_interface.h"

ComInterface::ComInterface(uint8_t port_num)
{
    _port = port_num;
    _buffer_pool = BufPoolManager::find("message");
    _recv_mail_box = MailboxManager::find<ComMessage>("receive");
}

bool ComInterface::match(uint8_t port_num)
{
    if (_port == port_num)
    {
        return true;
    }
    return false;
}

AC_RET ComInterface::receive(uint32_t len)
{
    ComMessage message;

    message.buf = _receive_buffer;
    message.port = _port;
    message.len = len;
    open();
    return _recv_mail_box->push(&message);
}


