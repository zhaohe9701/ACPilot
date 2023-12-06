/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-10-02 16:55:53
 * @LastEditors: zhaohe
 * @LastEditTime: 2023-03-12 21:24:01
 * @FilePath: \ZH_FLIGHT\Sys\Protocol\ibus.cpp
 * Copyright (C) 2022 zhaohe. All rights reserved.
 */

#include <string.h>
#include "ibus.h"
#include "config.h"
#include "Mail/mailbox.h"
#include "Remote/remote_data.h"
#include "Message/receive_server.h"

#define IBUS_HEAD1 0x20
#define IBUS_HEAD2 0x40

IbusParser::IbusParser()
{
    _manager = MailboxManager::find<RemoteData>("remote");
    MessageReceiveServer::add(this);
}

bool IbusParser::match(ComMessage &message)
{
    if (message.buf[0] == IBUS_HEAD1 && message.buf[1] == IBUS_HEAD2 && message.len == IBUS_LENGTH)
    {
        return true;
    }
    return false;
}

AC_RET IbusParser::parseMessage(ComMessage &message)
{
    RemoteData data;

    if (message.len != IBUS_LENGTH)
    {
        return AC_ERROR;
    }
    _channel_data[0]  = ((uint16_t)(message.buf[3]  & 0x0F) << 8) | message.buf[2];
    _channel_data[1]  = ((uint16_t)(message.buf[5]  & 0x0F) << 8) | message.buf[4];
    _channel_data[2]  = ((uint16_t)(message.buf[7]  & 0x0F) << 8) | message.buf[6];
    _channel_data[3]  = ((uint16_t)(message.buf[9]  & 0x0F) << 8) | message.buf[8];
    _channel_data[4]  = ((uint16_t)(message.buf[11] & 0x0F) << 8) | message.buf[10];
    _channel_data[5]  = ((uint16_t)(message.buf[13] & 0x0F) << 8) | message.buf[12];
    _channel_data[6]  = ((uint16_t)(message.buf[15] & 0x0F) << 8) | message.buf[14];
    _channel_data[7]  = ((uint16_t)(message.buf[17] & 0x0F) << 8) | message.buf[16];
    _channel_data[8]  = ((uint16_t)(message.buf[19] & 0x0F) << 8) | message.buf[18];
    _channel_data[9]  = ((uint16_t)(message.buf[21] & 0x0F) << 8) | message.buf[20];
    _channel_data[10] = ((uint16_t)(message.buf[23] & 0x0F) << 8) | message.buf[22];
    _channel_data[11] = ((uint16_t)(message.buf[25] & 0x0F) << 8) | message.buf[24];

    _channel_data[12] = ((uint16_t)message.buf[27] << 8) | message.buf[26];
    _channel_data[13] = ((uint16_t)message.buf[29] << 8) | message.buf[28];

    _channel_data[14] = ((uint16_t)(message.buf[3]  & 0xF0) >> 4) | (uint16_t)(message.buf[5]  & 0xF0) | ((uint16_t)(message.buf[7]  & 0xF0) << 4);
    _channel_data[15] = ((uint16_t)(message.buf[9]  & 0xF0) >> 4) | (uint16_t)(message.buf[11] & 0xF0) | ((uint16_t)(message.buf[13] & 0xF0) << 4);
    _channel_data[16] = ((uint16_t)(message.buf[15] & 0xF0) >> 4) | (uint16_t)(message.buf[17] & 0xF0) | ((uint16_t)(message.buf[19] & 0xF0) << 4);
    _channel_data[17] = ((uint16_t)(message.buf[21] & 0xF0) >> 4) | (uint16_t)(message.buf[23] & 0xF0) | ((uint16_t)(message.buf[25] & 0xF0) << 4);

    memcpy(data.channel, _channel_data, sizeof(uint16_t) * MAX_CHANNEL_NUM);
    _manager->update(&data);
    return AC_OK;
}


