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
#include "Control/AttitudeControl/default_attitude_controller.h"
#include "type.h"
#include <string.h>
#include "Debug/ac_debug.h"
#include "Receive/receive_server.h"

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

CrsfParser::CrsfParser()
{
    _manager = Mailbox<RemoteData>::find("remote");
}

bool CrsfParser::match(ComMessage &message)
{
    if (message.buf[0] == CRSF_HEAD && message.buf[1] == message.len - 2)
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
        data.channel[i] = (((uint32_t)data.channel[i] - CRSF_MIN) * REMOTE_MAX) / (CRSF_MAX - CRSF_MIN);
    }
    data.channel_num = (message.len - 4) * 16 / 11;
    AC_INFO("c1:%d c2:%d c3:%d c4:%d c5:%d c6:%d c7:%d c8:%d\n", data.channel[0], data.channel[1], data.channel[2], data.channel[3], data.channel[4], data.channel[5], data.channel[6], data.channel[7]);
    _manager->update(&data);
    return AC_OK;
}





