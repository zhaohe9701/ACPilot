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
#ifndef MESSAGE_PARSER_H_
#define MESSAGE_PARSER_H_

#include <stdint.h>
#include "Interface/com_interface.h"
#include "type.h"


class MessageReceiveParser
{
public:
    virtual AC_RET parseMessage(ComMessage &message) = 0;
    virtual bool match(ComMessage &message) = 0;
    virtual ~MessageReceiveParser()= default;
};

#endif