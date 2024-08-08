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

#define COMMAND_HEAD 0x3E

#include "Mailbox/mailbox.h"
#include "Receive/receive_parser.h"
#include "Type/type.h"

namespace Service
{

    enum CommandMethod
    {
        AT_GET = 0,
        AT_SET = 1,
        AT_DOWNLOAD = 2,
    };

    class CommandParser : virtual public MessageReceiveParser
    {
    public:
        CommandParser();

        bool match(ComMessage &messaged) override;

        AC_RET parse(ComMessage &message, bool &free_message) override;

    private:
        Utils::Mailbox<CommandMessage> *_manager = nullptr;
    };
}
#endif