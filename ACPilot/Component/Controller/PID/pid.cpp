//
// Created by zhaohe on 2024/4/29.
//

#include "pid.h"
#include "default_debug.h"

void Proportion::init(float gain)
{
    _gain = gain;
}

void Proportion::reset(float gain)
{
    _gain = gain;
}

float Proportion::calculate(float error)
{
    return _gain * error;
}

void Integral::init(float gain, float limit)
{
    _gain = gain;
    _limit = limit;
}

void Integral::reset(float gain, float limit)
{
    _gain = gain;
    _limit = limit;
    clear();
}

void Integral::clear()
{
    _sum = 0.f;
}

float Integral::calculate(float error, float dt)
{
    _sum += error * dt;
    if (_sum > _limit)
    {
        _sum = _limit;
    } else if (_sum < -_limit)
    {
        _sum = -_limit;
    }
    return _gain * _sum;
}


void Differential::init(float gain)
{
    _gain = gain;
}

void Differential::reset(float gain)
{
    _gain = gain;
}

float Differential::calculate(float error, float dt)
{
    float d = (error - _last_error) / dt;
    _last_error = error;
    return _gain * d;
}

float Differential::getGain()
{
    return _gain;
}

Pid::Pid(Filter *diff_filter)
{
    _diff_filter = diff_filter;
}

void Pid::init(float kp, float ki, float kd, float out_limit, float int_limit)
{
    BASE_INFO("PID kp:%f ki:%f kd:%f out_limit:%f int_limit:%f", kp, ki, kd, out_limit, int_limit);
    _proportion.init(kp);
    _integral.init(ki, int_limit);
    _differential.init(kd);
    _out_limit = out_limit;
}

float Pid::output(float error, float dt)
{
    float p = _proportion.calculate(error);
    float i = _integral.calculate(error, dt);
    float d = _differential.calculate(error, dt);

    if (_diff_filter != nullptr)
    {
        d = _diff_filter->apply(d);
    }

    float out = p + i + d;

    if (out > _out_limit)
    {
        out = _out_limit;
    } else if (out < -_out_limit)
    {
        out = -_out_limit;
    }

    return out;
}

void Pid::reset(float kp, float ki, float kd, float out_limit, float int_limit)
{
    _proportion.reset(kp);
    _integral.reset(ki, int_limit);
    _differential.reset(kd);
    _out_limit = out_limit;
}

void Pid::clear()
{
    _integral.clear();
}

float Pid::output(float error, float d_error, float dt)
{
    float p = _proportion.calculate(error);
    float i = _integral.calculate(error, dt);
    float d = _differential.getGain() * d_error;

    if (_diff_filter != nullptr)
    {
        d = _diff_filter->apply(d);
    }

    float out = p + i + d;

    if (out > _out_limit)
    {
        out = _out_limit;
    } else if (out < -_out_limit)
    {
        out = -_out_limit;
    }

    return out;
}
