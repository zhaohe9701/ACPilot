//
// Created by zhaohe on 2024/4/6.
//
#include "receive_parser.h"
#include "receive_server.h"

MessageReceiveParser::MessageReceiveParser()
{
    MessageReceiveServer::add(this);
}
