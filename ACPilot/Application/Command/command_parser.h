/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2023-02-25 01:43:38
 * @LastEditors: zhaohe
 * @LastEditTime: 2023-03-27 00:06:06
 * @FilePath: \ZH_FLIGHT\Sys\Command\command.h
 * Copyright (C) 2023 zhaohe. All rights reserved.
 */
#ifndef COMMAND_PARSER_H_
#define COMMAND_PARSER_H_

#define MAX_URL_LEN 128
#define MAX_JSON_LEN 512
#define MAX_ARG_NUM 8
#define COMMAND_HEAD 0x3E

#include "Mail/mailbox.h"
#include "Message/message_parser.h"
#include "type.h"

enum CommandMethod
{
    AT_GET = 0,
    AT_SET = 1,
    AT_DOWNLOAD = 2,
};
class CommandMessage
{
public:
    uint8_t argc = 0;
    char *argv[MAX_ARG_NUM] = {nullptr};
    ComMessage *message;
};

class CommandParser : virtual public MessageReceiveParser
{
public:
    CommandParser();
    bool match(ComMessage &messaged) override;
    AC_RET parseMessage(ComMessage &message) override;
private:
    Mailbox<CommandMessage> *_manager = nullptr;
};

#endif