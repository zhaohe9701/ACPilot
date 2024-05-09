//
// Created by zhaohe on 2024/4/23.
//

#include "motor.h"

Motor *Motor::_group[MAX_MOTOR_NUM] = {nullptr};

Motor::Motor(uint8_t id)
{
    _id = id;
    _group[_id] = this;
}

AC_RET Motor::init()
{
    return AC_OK;
}

AC_RET Motor::set(uint8_t id, float value)
{
    if (id >= MAX_MOTOR_NUM || _group[id] == nullptr)
    {
        return AC_ERROR;
    }
    if (value > 100.f)
    {
        value = 100.f;
    }
    if (value < 0.f)
    {
        value = 0.f;
    }
    return _group[id]->set(value);
}

AC_RET Motor::enable()
{
    return AC_OK;
}

AC_RET Motor::disable()
{
    return AC_OK;
}

void Motor::stopAll()
{
    for(int i = 0; i < MAX_MOTOR_NUM; i++)
    {
        if(_group[i] != nullptr)
        {
            _group[i]->set(0.F);
        }
    }
}

void Motor::enableAll()
{
    for (int i = 0; i < MAX_MOTOR_NUM; i++)
    {
        if (_group[i] != nullptr)
        {
            _group[i]->enable();
        }
    }
}

void Motor::disableAll()
{
    for (int i = 0; i < MAX_MOTOR_NUM; i++)
    {
        if (_group[i] != nullptr)
        {
            _group[i]->disable();
        }
    }
}

