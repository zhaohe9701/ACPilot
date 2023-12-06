#include <string.h>
#include "remote_channel.h"
#include "Json/json_tree.h"
#include "config.h"
#include "type.h"
#include "Memory/ac_memory.h"


void RemoteChannel::init(const char *name, ChannelType type, uint8_t channel, uint16_t thr1, uint16_t thr2)
{
    strncpy(_name, name , PARAM_NAME_LEN - 1);
    _type = type;
    _channel = channel;
    _thr1 = thr1;
    _thr2 = thr2;
}

uint16_t RemoteChannel::map(uint16_t v)
{
    if (ANALOG == _type)
    {
        return v;
    } else if (SWITCH1 == _type)
    {
        if (v < _thr1)
        {
            return (uint16_t) _switch_map[0];
        } else
        {
            return (uint16_t) _switch_map[1];
        }
    } else
    {
        if (v < _thr1)
        {
            return (uint16_t) _switch_map[0];
        } else if (v < _thr2)
        {
            return (uint16_t) _switch_map[1];
        } else
        {
            return (uint16_t) _switch_map[2];
        }
    }
}

uint8_t RemoteChannel::getChannel()
{
    return _channel;
}

JsonTree* RemoteChannel::createIndex()
{
    JsonTree* root = new JsonTree();
    root->addData(nullptr, AC_STRUCT, _name);
    JsonTree* node = nullptr;

    node = new JsonTree();
    node->addData(&_type, AC_UINT8, "channel");
    root->addChild(node);

    node = new JsonTree();
    node->addData(&_type, AC_UINT8, "type");
    root->addChild(node);

    if (ANALOG == _type)
    {
        return root;
    }

    node = new JsonTree();
    node->addData(&_thr1, AC_UINT16, "thr1");
    root->addChild(node);

    if (SWITCH2 == _type)
    {
        node = new JsonTree();
        node->addData(&_thr2, AC_UINT16, "thr2");
        root->addChild(node);
    }

    JsonTree* array = new JsonTree();
    array->addData(nullptr, AC_ARRAY, "switch_map");
    root->addChild(array);

    node = new JsonTree();
    node->addData(&_switch_map[0], AC_UINT8, "0");
    array->addChild(node);
    node = new JsonTree();
    node->addData(&_switch_map[1], AC_UINT8, "1");
    array->addChild(node);
    if (SWITCH2 == _type)
    {
        node = new JsonTree();
        node->addData(&_switch_map[2], AC_UINT8, "2");
        array->addChild(node);
    }
    return root;
}

RemoteChannelMapper::RemoteChannelMapper()
{
    _map[MODE_CHANNEL].init("mode", SWITCH2, 5, 3333, 6666);
    _map[LOCK_CHANNEL].init("lock", SWITCH1, 4, 5000);
    _map[PITCH_CHANNEL].init("pitch", ANALOG, 2);
    _map[ROLL_CHANNEL].init("roll", ANALOG, 1);
    _map[YAW_CHANNEL].init("yaw", ANALOG, 3);
    _map[THROTTLE_CHANNEL].init("throttle", ANALOG, 0);
    _map[X_CHANNEL].init("x", ANALOG, 2);
    _map[Y_CHANNEL].init("y", ANALOG, 1);
    _map[Z_CHANNEL].init("z", ANALOG, 0);
}

JsonTree* RemoteChannelMapper::createIndex()
{
    JsonTree *root = new JsonTree();
    root->addData(nullptr, AC_STRUCT, "channel_map");
    JsonTree *node = nullptr;
    for (int i = 0; i < CHANNEL_MAP_LEN; ++i)
    {
        node = _map[i].createIndex();
        root->addChild(node);
    }
    return root;
}

AC_RET RemoteChannelMapper::map(RemoteData &remote_data, UsualState &usual_state)
{
    if (_map[LOCK_CHANNEL].getChannel() > remote_data.channel_num)
    {
        return AC_ERROR;
    }
    usual_state.lock = (FlyLock)_map[LOCK_CHANNEL].map(remote_data.channel[_map[LOCK_CHANNEL].getChannel()]);

    if (_map[MODE_CHANNEL].getChannel() > remote_data.channel_num)
    {
        return AC_ERROR;
    }
    usual_state.mode = (Mode)_map[MODE_CHANNEL].map(remote_data.channel[_map[MODE_CHANNEL].getChannel()]);
    return AC_OK;
}

AC_RET RemoteChannelMapper::map(RemoteData &remote_data, ExpectAttitudeState &attitude_state)
{
    if (_map[PITCH_CHANNEL].getChannel() > remote_data.channel_num)
    {
        return AC_ERROR;
    }
    attitude_state.euler.x = _map[PITCH_CHANNEL].map(remote_data.channel[_map[PITCH_CHANNEL].getChannel()]);

    if (_map[ROLL_CHANNEL].getChannel() > remote_data.channel_num)
    {
        return AC_ERROR;
    }
    attitude_state.euler.y = _map[ROLL_CHANNEL].map(remote_data.channel[_map[ROLL_CHANNEL].getChannel()]);

    if (_map[YAW_CHANNEL].getChannel() > remote_data.channel_num)
    {
        return AC_ERROR;
    }
    attitude_state.euler.z = _map[YAW_CHANNEL].map(remote_data.channel[_map[YAW_CHANNEL].getChannel()]);

    return AC_OK;
}

AC_RET RemoteChannelMapper::map(RemoteData &remote_data, Altitude &altitude)
{
    if (_map[THROTTLE_CHANNEL].getChannel() > remote_data.channel_num)
    {
        return AC_ERROR;
    }
    altitude = _map[THROTTLE_CHANNEL].map(remote_data.channel[_map[THROTTLE_CHANNEL].getChannel()]);

    return AC_OK;
}

AC_RET RemoteChannelMapper::map(RemoteData &remote_data, ExpectPositionState &position_state)
{
    if (_map[X_CHANNEL].getChannel() > remote_data.channel_num)
    {
        return AC_ERROR;
    }
    position_state.coordinate.x = _map[X_CHANNEL].map(remote_data.channel[_map[X_CHANNEL].getChannel()]);

    if (_map[Y_CHANNEL].getChannel() > remote_data.channel_num)
    {
        return AC_ERROR;
    }
    position_state.coordinate.y = _map[Y_CHANNEL].map(remote_data.channel[_map[Y_CHANNEL].getChannel()]);

    if (_map[Z_CHANNEL].getChannel() > remote_data.channel_num)
    {
        return AC_ERROR;
    }
    position_state.coordinate.z = _map[Z_CHANNEL].map(remote_data.channel[_map[Z_CHANNEL].getChannel()]);

    if (_map[YAW_CHANNEL].getChannel() > remote_data.channel_num)
    {
        return AC_ERROR;
    }
    position_state.yaw = _map[YAW_CHANNEL].map(remote_data.channel[_map[YAW_CHANNEL].getChannel()]);
    return AC_OK;
}