/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2023-03-12 23:22:56
 * @LastEditors: zhaohe
 * @LastEditTime: 2023-03-13 00:51:27
 * @FilePath: \ZH_FLIGHT\Sys\Protocol\crsf.cpp
 * Copyright (C) 2023 zhaohe. All rights reserved.
 */
#include "crsf.h"
#include "Type/type.h"
#include <string.h>
#include "Debug/default_debug.h"


#ifdef C_ESP32

using namespace Component;

#pragma pack(1)
struct CrsfPacket
{
    unsigned ch0: 11 = 0;
    unsigned ch1: 11 = 0;
    unsigned ch2: 11 = 0;
    unsigned ch3: 11 = 0;
    unsigned ch4: 11 = 0;
    unsigned ch5: 11 = 0;
    unsigned ch6: 11 = 0;
    unsigned ch7: 11 = 0;
    unsigned ch8: 11 = 0;
    unsigned ch9: 11 = 0;
    unsigned ch10: 11 = 0;
    unsigned ch11: 11 = 0;
    unsigned ch12: 11 = 0;
    unsigned ch13: 11 = 0;
    unsigned ch14: 11 = 0;
    unsigned ch15: 11 = 0;
};
#pragma pack()

AC_RET CrsfParser::_decode(const uint8_t *bin, uint16_t *axis, int bit_num, int len)
{
    CrsfPacket packet;
    memcpy(&packet, bin, len);
    axis[0] = packet.ch0;
    axis[1] = packet.ch1;
    axis[2] = packet.ch2;
    axis[3] = packet.ch3;
    axis[4] = packet.ch4;
    axis[5] = packet.ch5;
    axis[6] = packet.ch6;
    axis[7] = packet.ch7;
    axis[8] = packet.ch8;
    axis[9] = packet.ch9;
    axis[10] = packet.ch10;
    axis[11] = packet.ch11;
    axis[12] = packet.ch12;
    axis[13] = packet.ch13;
    axis[14] = packet.ch14;
    axis[15] = packet.ch15;
    return AC_OK;
}

#else
#define getBitByPosition(x, n) (((x) >> (n)) & 1)

AC_RET CrsfParser::_decode(const uint8_t *bin, uint16_t *axis, int bit_num, int len)
{
    for (int i = 0; i < 8 * len; ++i)
    {
        int bi = i / 8;
        int bp = (i % 8);

        int ai = i / bit_num;
        int ap = (i % bit_num);

        axis[ai] = axis[ai] | (getBitByPosition(bin[bi], bp) << ap);
    }
    return AC_OK;
}
#endif

CrsfParser::CrsfParser()
{
    _mailbox = Utils::Mailbox<RemoteData>::find("remote");
}

bool CrsfParser::match(ComMessage &message)
{
    if (message.buf[0] == CRSF_HEAD && message.buf[1] == message.len - 2 && message.buf[2] == CRSF_PAYLOAD)
    {
        return true;
    }
    return false;
}

AC_RET CrsfParser::parse(ComMessage &message, bool &free_message)
{
    RemoteData data;

    uint16_t axis[CRSF_CHANNEL_NUM] = {0};

    _decode(message.buf + 3, axis, CRSF_BIT_LEN, message.buf[1] - 2);

    memcpy(data.channel, axis, sizeof(uint16_t) * MAX_CHANNEL_NUM);
    for (int i = 0; i < MAX_CHANNEL_NUM; ++i)
    {
        data.channel[i] = (((uint32_t) data.channel[i] - CRSF_MIN) * REMOTE_MAX) / (CRSF_MAX - CRSF_MIN);
    }
    data.channel_num = (message.len - 4) * 16 / 11;
//
//    BASE_INFO("c1:%d c2:%d c3:%d c4:%d c5:%d c6:%d c7:%d c8:%d\n", data.channel[0], data.channel[1],
//              data.channel[2], data.channel[3], data.channel[4], data.channel[5], data.channel[6], data.channel[7]);
    _mailbox->update(&data);
    return AC_OK;
}





