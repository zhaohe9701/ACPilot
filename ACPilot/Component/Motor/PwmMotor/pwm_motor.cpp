//
// Created by zhaohe on 2024/4/23.
//

#include "pwm_motor.h"
#include "default_debug.h"

PwmMotor::PwmMotor(Pwm *pwm, uint8_t id) : Motor(id)
{
    if (pwm == nullptr)
    {
        BASE_ERROR("pwm driver is nullptr");
    }
    _pwm = pwm;
}

AC_RET PwmMotor::set(float value)
{
    return _pwm->set(value);
}
