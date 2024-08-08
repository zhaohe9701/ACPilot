//
// Created by zhaohe on 2024/4/26.
//

#include "remote_mapper.h"
#include "DataModule/data_module.h"
#include "error_handing.h"
#include "Debug/default_debug.h"

#define SWITCH_TYPE_NUM 2

using namespace Component;

uint8_t RemoteMapper::_map[MAX_CHANNEL_NUM] = {0};
uint8_t RemoteMapper::_sw[MAX_CHANNEL_NUM] = {0};
uint16_t RemoteMapper::_thr1[1] = {0};
uint16_t RemoteMapper::_thr2[2] = {0};


RemoteMapper::RemoteMapper(uint8_t id)
{
    if (id >= MAX_CHANNEL_NUM)
    {
        BASE_ERROR("remote mapper id out of range");
    }
    _id = id;
}

AC_RET RemoteMapper::init(const char *url)
{
    RemoteConfig config;
    RETURN_CHECK(Framework::DataModule::read(url, &config, sizeof(RemoteConfig)));

    for (int i = 0; i < MAX_CHANNEL_NUM; i++)
    {
        if (MAX_CHANNEL_NUM <= config.map[i])
        {
            BASE_ERROR("remote mapper map out of range");
            return AC_ERROR;
        }
        if (SWITCH_TYPE_NUM < config.sw[i])
        {
            BASE_ERROR("remote mapper sw out of range");
            return AC_ERROR;
        }
        _map[i] = config.map[i];
        _sw[i] = config.sw[i];
    }
    _thr1[0] = config.thr[0];
    _thr2[0] = config.thr[1];
    _thr2[1] = config.thr[2];

    return AC_OK;
    error:
    BASE_ERROR("remote mapper init failed");
    return AC_ERROR;
}

uint16_t RemoteMapper::map(RemoteData &remote_data)
{
    _old = _new;
    if (_sw[_id] == 0)
    {
        _new = remote_data.channel[_map[_id]];
    } else
    {
        _new = _switchMap(remote_data.channel[_map[_id]], _sw[_id]);
    }
    return _new;
}

uint16_t RemoteMapper::_switchMap(uint16_t val, uint8_t switch_type)
{
    switch (switch_type)
    {
        case 1:
            return val <= _thr1[0] ? 0 : 1;
        case 2:
            for (int i = 0; i < 2; i++)
            {
                if (val <= _thr2[i])
                {
                    return i;
                }
            }
            return 2;
        default:
            BASE_ERROR("remote mapper switch type out of range");
            return 10001;
    }
}

bool RemoteMapper::isChanged() const
{
    if (_old != _new)
    {
        return true;
    }
    return false;
}

