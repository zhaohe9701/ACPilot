//
// Created by zhaohe on 2023/8/19.
//
#include "com_interface.h"

List<ComInterface *> ComInterface::_list;

ComInterface::ComInterface(uint8_t port_num)
{
    _port = port_num;
    _recv_mail_box = Mailbox<ComMessage>::find("receive");
    add(this);
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

    message.buf = _recv_buffer;
    message.port = _port;
    message.len = len;
    message.pool = _recv_pool;
    open();
    return _recv_mail_box->push(&message, AC_IMMEDIATELY);
}

void ComInterface::add(ComInterface *com)
{
    _list.pushBack(com);
}

List<ComInterface *> * ComInterface::getList()
{
    return &_list;
}

AC_RET ComInterface::init()
{
    return AC_OK;
}

AC_RET ComInterface::open()
{
    return AC_OK;
}




