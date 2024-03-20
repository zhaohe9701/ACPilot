//
// Created by zhaohe on 2023/11/6.
//
#include "transmit_server.h"
#include "sys.h"

List<ComInterface*> MessageTransmitServer::_interface_list;
Mailbox<ComMessage>* MessageTransmitServer::_mailbox = nullptr;
MemoryPool *MessageTransmitServer::_buffer_pool = nullptr;

AC_RET MessageTransmitServer::init()
{
    _buffer_pool = MemoryPoolManager::find("message");
    _mailbox = MailboxManager::find<ComMessage>("send");
    if (nullptr == _buffer_pool || nullptr == _mailbox)
    {
        return AC_ERROR;
    }
    return AC_OK;
}

void MessageTransmitServer::add(ComInterface *interface)
{
    _interface_list.pushBack(interface);
}

AC_RET MessageTransmitServer::run()
{
    ComMessage message;
    _mailbox->pop(&message);
    uint8_t port = message.port;
    for (ListNode<ComInterface*> *it = _interface_list.begin(); it != _interface_list.end(); it = it->getNext())
    {
        if ((*(*it))->match(port))
        {
            (*(*it))->send(message.buf, message.len, AC_FOREVER);
            return AC_OK;
        }
    }
    return AC_ERROR;
}
