//
// Created by zhaohe on 2023/8/19.
//
#include "com_interface.h"

List<Com *> Com::_list;

Com::Com(uint8_t port_num)
{
    _port = port_num;
    _recv_mail_box = Mailbox<ComMessage>::find("receive");
    add(this);
}

bool Com::match(uint8_t port_num)
{
    if (_port == port_num)
    {
        return true;
    }
    return false;
}

AC_RET Com::receive(uint32_t len)
{
    ComMessage message;

    message.buf = _recv_buffer;
    message.port = _port;
    message.len = len;
    message.pool = _recv_pool;
    open();
    return _recv_mail_box->push(&message, AC_IMMEDIATELY);
}

void Com::add(Com *com)
{
    _list.pushBack(com);
}

List<Com *> * Com::getList()
{
    return &_list;
}

AC_RET Com::init()
{
    return AC_OK;
}

AC_RET Com::open()
{
    return AC_OK;
}




