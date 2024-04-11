/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-09-27 23:06:21
 * @LastEditors: zhaohe
 * @LastEditTime: 2023-03-12 21:17:55
 * @FilePath: \ZH_FLIGHT\Sys\Protocol\ibus.h
 * Copyright (C) 2022 zhaohe. All rights reserved.
 */
#ifndef IBUS_H_
#define IBUS_H_

#include "message.h"
#include "Mail/mailbox.h"
#include "Remote/remote_data.h"
#include "Receive/receive_parser.h"
#include "AircraftState/aircraft_state.h"

#define IBUS_HEAD1          0x20
#define IBUS_HEAD2          0x40
#define IBUS_LENGTH         32
#define IBUS_CHANNEL_NUM    18

#define IBUS_SWITCH_0       1000
#define IBUS_SWITCH_1       1500
#define IBUS_SWITCH_2       2000

#define IBUS_TRANS_CHANNEL_FROM_INT_TO_FLOAT(x) ((float)((x) - 1500) / 1500.0f)



class IbusParser : virtual public MessageReceiveParser
{
public:
    IbusParser();
    bool match(ComMessage &message) override;
    AC_RET parse(ComMessage &message, bool &free_message) override;
private:
    uint16_t _channel_data[IBUS_CHANNEL_NUM] = {0};
    Mailbox<RemoteData> *_manager = nullptr;
};
#endif