/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-10-21 23:47:30
 * @LastEditors: zhaohe
 * @LastEditTime: 2023-03-27 01:15:07
 * @FilePath: \ZH_FLIGHT\Sys\Message\message_server.cpp
 * Copyright (C) 2022 zhaohe. All rights reserved.
 */


#include "receive_parser.h"
#include "Type/type.h"
#include "receive_server.h"
#include "Debug/default_debug.h"

using namespace Service;

Utils::Mailbox<ComMessage> *MessageReceiveServer::_mailbox = nullptr;
Osal::AcThread *MessageReceiveServer::_receive_task = nullptr;
Common::List<MessageReceiveParser *> MessageReceiveServer::_parser_list;

AC_RET MessageReceiveServer::init()
{
    _mailbox = Utils::Mailbox<ComMessage>::find("receive");
    if (nullptr == _mailbox)
    {
        BASE_ERROR("object can't find");
        return AC_ERROR;
    }
    _receive_task = new Osal::AcThread("receive", RECEIVE_TASK_STACK_SIZE, RECEIVE_TASK_PRIO, RECEIVE_TASK_CORE);
    return AC_OK;
}

AC_RET MessageReceiveServer::start()
{
    if (nullptr == _receive_task)
    {
        BASE_ERROR("service not init");
        return AC_ERROR;
    }
    _receive_task->run(_loop, nullptr);
    return AC_OK;
}

void MessageReceiveServer::add(MessageReceiveParser *parser)
{
    _parser_list.pushBack(parser);
}

void MessageReceiveServer::_loop(void *param)
{
    if (nullptr == _mailbox)
    {
        BASE_ERROR("NULL ptr");
        Osal::AcThread::killSelf();
    }
    for (;;)
    {
        ComMessage message;
        bool free_message = true;
        if (AC_OK != _select(&message))
        {
            continue;
        }
        for (Common::ListNode<MessageReceiveParser *> *it = _parser_list.begin(); it != _parser_list.end(); it = it->getNext())
        {
            if ((*(*it))->match(message))
            {
                (*(*it))->parse(message, free_message);
                break;
            }
        }
        if (free_message)
        {
            if (nullptr != message.pool)
            {
                static_cast<Utils::MemoryPool *>(message.pool)->free(message.buf);
            } else
            {
                BASE_ERROR("pool is null");
            }
        }
    }
}

AC_RET MessageReceiveServer::_select(ComMessage *message, uint32_t timeout)
{
    return _mailbox->pop(message, timeout);
}
