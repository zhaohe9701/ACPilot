//
// Created by zhaohe on 2023/11/6.
//
#include "transmit_server.h"
#include "Debug/default_debug.h"

using namespace Service;

Common::List<Interface::Com *> *MessageTransmitServer::_interface_list;
Utils::Mailbox<ComMessage> *MessageTransmitServer::_mailbox = nullptr;
Osal::AcThread *MessageTransmitServer::_transmit_task = nullptr;

AC_RET MessageTransmitServer::init()
{
    _mailbox = Utils::Mailbox<ComMessage>::find("send");
    if (nullptr == _mailbox)
    {
        BASE_ERROR("object can't find");
        return AC_ERROR;
    }
    _transmit_task = new Osal::AcThread("transmit", SEND_TASK_STACK_SIZE, SEND_TASK_PRIO, SEND_TASK_CORE);
    return AC_OK;
}

AC_RET MessageTransmitServer::start()
{
    _interface_list = Interface::Com::getList();
    if (nullptr == _transmit_task)
    {
        BASE_ERROR("service not init");
        return AC_ERROR;
    }
    _transmit_task->run(MessageTransmitServer::_loop, nullptr);
    return AC_OK;
}

void MessageTransmitServer::_loop(void *param)
{
    if (nullptr == _mailbox)
    {
        BASE_ERROR("NULL ptr");
        Osal::AcThread::killSelf();
    }
    for (;;)
    {
        ComMessage message;
        if (AC_OK != _mailbox->pop(&message, AC_FOREVER))
        {
            continue;
        }
        for (Common::ListNode<Interface::Com *> *it = _interface_list->begin(); it != _interface_list->end(); it = it->getNext())
        {
            if ((*(*it))->match(message.port))
            {
                (*(*it))->send(message.buf, message.len, AC_FOREVER);
                break;
            }
        }
        if (nullptr != message.pool)
        {
            static_cast<Utils::MemoryPool *>(message.pool)->free(message.buf);
        } else
        {
            BASE_ERROR("pool is null");
        }
    }
}


