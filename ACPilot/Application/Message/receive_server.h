/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-10-21 23:28:46
 * @LastEditors: zhaohe
 * @LastEditTime: 2023-03-26 23:48:11
 * @FilePath: \ZH_FLIGHT\Sys\Message\message_server.h
 * Copyright (C) 2022 zhaohe. All rights reserved.
 */
#ifndef MESSAGE_SERVER_H_
#define MESSAGE_SERVER_H_


#include "config.h"
#include "type.h"
#include "Interface/com_interface.h"
#include "message_parser.h"
#include "Queue/ac_queue.h"
#include "Mail/mailbox.h"
#include "List/ac_list.h"

class MessageReceiveServer
{
public:
    static AC_RET init();
    static void add(MessageReceiveParser *interface);
    static AC_RET run();
private:
    static AC_RET _select(ComMessage *message, uint32_t timeout = AC_FOREVER);
    static List<MessageReceiveParser*> _parser_list;
    static MemoryPool *_buffer_pool;
    static Mailbox<ComMessage> *_mailbox;
};

#endif
