//
// Created by zhaohe on 2023/6/18.
//

#ifndef MESSAGE_H_
#define MESSAGE_H_

#include "type.h"
#include "config.h"

#define MAX_ARG_NUM 8

class ComMessage
{
public:
    uint8_t port = 0x00;
    uint8_t *buf = nullptr;
    uint32_t len = 0;
    void *pool = nullptr;
};

class CommandMessage
{
public:
    uint8_t argc = 0;
    char *argv[MAX_ARG_NUM] = {nullptr};
    ComMessage message;
};

#define REMOTE_MAX  10000

class RemoteData
{
public:
    uint16_t channel[MAX_CHANNEL_NUM] = {0};
    uint16_t channel_num  = 0;
};

#endif
