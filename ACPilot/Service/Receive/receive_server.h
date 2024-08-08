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
#include "Type/type.h"
#include "Com/com.h"
#include "receive_parser.h"
#include "Queue/queue.h"
#include "Mailbox/mailbox.h"
#include "List/ac_list.h"
#include "Thread/thread.h"

namespace Service
{
    class MessageReceiveServer
    {
    public:
        static AC_RET init();

        static AC_RET start();

        static void add(MessageReceiveParser *parser);

    private:
        static AC_RET _select(ComMessage *message, uint32_t timeout = AC_FOREVER);

        static Common::List<MessageReceiveParser *> _parser_list;
        static Utils::Mailbox<ComMessage> *_mailbox;
        static Osal::AcThread *_receive_task;

        static void _loop(void *param);
    };
}
#endif
