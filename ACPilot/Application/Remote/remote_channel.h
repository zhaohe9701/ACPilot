/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2023-03-12 19:32:29
 * @LastEditors: zhaohe
 * @LastEditTime: 2023-03-12 19:41:06
 * @FilePath: \ZH_FLIGHT\Sys\Remote\remote_channel.h
 * Copyright (C) 2023 zhaohe. All rights reserved.
 */
#ifndef REMOTE_CHANNEL_H_
#define REMOTE_CHANNEL_H_

#include "config.h"
#include "type.h"
#include "remote_data.h"
#include "AircraftState/aircraft_state.h"

enum ChannelType
{
    ANALOG,
    SWITCH1,
    SWITCH2,
    THROW_NUM
};
class RemoteChannel
{
private:
    char _name[PARAM_NAME_LEN] = {0};
    uint8_t _channel = 0;
    uint8_t _type = ANALOG;
    uint8_t _switch_map[THROW_NUM] = {0,1,2};
    uint16_t _thr1 = 0;
    uint16_t _thr2 = 0;
public:
    void init(const char *name, ChannelType type, uint8_t channel, uint16_t thr1 = 0, uint16_t thr2 = 0);
    uint16_t map(uint16_t v);
    uint8_t getChannel();
};

class RemoteChannelMapper
{
private:
    RemoteChannel _map[CHANNEL_MAP_LEN];
public:
    RemoteChannelMapper();
    AC_RET map(RemoteData &remote_data, UsualState &usual_state);
    AC_RET map(RemoteData &remote_data, ExpectAttitudeState &attitude_state);
    AC_RET map(RemoteData &remote_data, Altitude &altitude);
    AC_RET map(RemoteData &remote_data, ExpectPositionState &position_state);
    ~RemoteChannelMapper() = default;
};

#endif
