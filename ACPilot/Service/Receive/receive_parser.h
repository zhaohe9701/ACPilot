/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2023-01-10 23:11:06
 * @LastEditors: zhaohe
 * @LastEditTime: 2023-03-26 23:52:11
 * @FilePath: \ZH_FLIGHT\Sys\Message\message_parser.h
 * Copyright (C) 2023 zhaohe. All rights reserved.
 */
#ifndef RECEIVE_PARSER_H_
#define RECEIVE_PARSER_H_

#include <stdint.h>
#include "Com/com.h"
#include "Type/type.h"
#include "message.h"

namespace Service
{
    class MessageReceiveParser
    {
    public:
        MessageReceiveParser();

        virtual AC_RET parse(ComMessage &message, bool &free_message) = 0;

        virtual bool match(ComMessage &message) = 0;

        virtual ~MessageReceiveParser() = default;
    };
}
#endif