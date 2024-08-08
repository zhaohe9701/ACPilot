//
// Created by zhaohe on 2024/4/6.
//
#include "receive_parser.h"
#include "receive_server.h"

using namespace Service;

MessageReceiveParser::MessageReceiveParser()
{
    MessageReceiveServer::add(this);
}
