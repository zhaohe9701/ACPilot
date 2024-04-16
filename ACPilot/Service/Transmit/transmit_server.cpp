//
// Created by zhaohe on 2023/11/6.
//
#include "transmit_server.h"
#include "default_debug.h"

#define SEND_TASK_PRIO  24
#define SEND_TASK_STACK_SIZE 2000

List<Com *> *MessageTransmitServer::_interface_list;
Mailbox<ComMessage> *MessageTransmitServer::_mailbox = nullptr;
AcThread *MessageTransmitServer::_transmit_task = nullptr;

AC_RET MessageTransmitServer::init()
{
    _mailbox = Mailbox<ComMessage>::find("send");
    if (nullptr == _mailbox)
    {
        BASE_ERROR("object can't find");
        return AC_ERROR;
    }
    _transmit_task = new AcThread("transmit", SEND_TASK_STACK_SIZE, SEND_TASK_PRIO);
    return AC_OK;
}

AC_RET MessageTransmitServer::start()
{
    _interface_list = Com::getList();
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
        AcThread::killSelf();
    }
    for (;;)
    {
        ComMessage message;
        if (AC_OK != _mailbox->pop(&message, AC_FOREVER))
        {
            continue;
        }
        for (ListNode<Com *> *it = _interface_list->begin(); it != _interface_list->end(); it = it->getNext())
        {
            if ((*(*it))->match(message.port))
            {
                (*(*it))->send(message.buf, message.len, AC_FOREVER);
                break;
            }
        }
        if (nullptr != message.pool)
        {
            static_cast<MemoryPool *>(message.pool)->free(message.buf);
        } else
        {
            BASE_ERROR("pool is null");
        }
    }
}


