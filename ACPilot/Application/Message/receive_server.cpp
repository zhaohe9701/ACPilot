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


#include "message_parser.h"
#include "type.h"
#include "receive_server.h"
#include "sys.h"

BufPool *MessageReceiveServer::_buffer_pool = nullptr;
Mailbox<ComMessage> *MessageReceiveServer::_mailbox = nullptr;
List<MessageReceiveParser*> MessageReceiveServer::_parser_list;

AC_RET MessageReceiveServer::init()
{
    _buffer_pool = BufPoolManager::find("message");
    _mailbox = MailboxManager::find<ComMessage>("receive");
    if (nullptr == _buffer_pool || nullptr == _mailbox)
    {
        return AC_ERROR;
    }
    return AC_OK;
}

void MessageReceiveServer::add(MessageReceiveParser *interface)
{
    _parser_list.pushBack(interface);
}

AC_RET MessageReceiveServer::run()
{
    ComMessage message;
    _select(&message);
    for (ListNode<MessageReceiveParser*> *it = _parser_list.begin(); it != _parser_list.end(); it = it->getNext())
    {
        if ((*(*it))->match(message))
        {
            (*(*it))->parseMessage(message);
            _buffer_pool->free(message.buf);
            return AC_OK;
        }
    }
    _buffer_pool->free(message.buf);
    return AC_ERROR;
}

AC_RET MessageReceiveServer::_select(ComMessage *message, uint32_t timeout)
{
    return _mailbox->pop(message, timeout);
}
