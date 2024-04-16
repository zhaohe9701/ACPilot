//
// Created by zhaohe on 2023/5/22.
//

#include "light.h"
#include "Memory/ac_memory.h"
#include "os.h"

Led::Led(Gpio* led, LightActiveLevel level)
{
    _led = led;
    _active_level = level;
}

void Led::setPattern(LightPattern pattern)
{
    _pattern = pattern;
}

void Led::_breathe()
{
    uint16_t width = 10;
    uint16_t hight_count = 0;
    uint16_t continue_time = 0;
    uint16_t time = 0;
    while (_pattern == BREATHE)
    {
        continue_time = 150;
        while (_pattern == BREATHE && time < continue_time)
        {
            hight_count = time * 10 / continue_time;
            _openLed();
            tickSleep(hight_count);
            _closeLed();
            tickSleep(width - hight_count);
            time++;
        }
        time = 0;
        tickSleep(500);
        continue_time = 150;
        while (_pattern == BREATHE && time < continue_time)
        {
            hight_count = time * 10 / continue_time;
            _openLed();
            tickSleep(width - hight_count);
            _closeLed();
            tickSleep(hight_count);
            time++;
        }
        tickSleep(500);
    }
}

void Led::_slowFlashing()
{
    while (_pattern == SLOW_FLASHING)
    {
        _closeLed();
        tickSleep(1000);
        if (_pattern != SLOW_FLASHING) break;
        _openLed();
        tickSleep(1000);
    }
}

void Led::_fastFlashing()
{
    while (_pattern == FAST_FLASHING)
    {
        _closeLed();
        tickSleep(200);
        _openLed();
        tickSleep(200);
    }
}

void Led::_pulseFlashing()
{
    while (_pattern == PULSE_FLASHING)
    {
        _closeLed();
        tickSleep(900);
        _openLed();
        tickSleep(100);
    }
}

void Led::_doublePulseFlashing()
{
    while (_pattern == DOUBLE_PULSE_FLASHING)
    {
        _closeLed();
        tickSleep(700);
        _openLed();
        tickSleep(100);
        _closeLed();
        tickSleep(100);
        _openLed();
        tickSleep(100);
    }
}

void Led::_keepOn()
{
    _openLed();
    while (_pattern == KEEP_ON)
    {
        tickSleep(100);
    }
}

void Led::_keepOff()
{
    _closeLed();
    while (_pattern == KEEP_OFF)
    {

        tickSleep(100);
    }
}

void Led::_openLed()
{
    if (LowActive == _active_level)
    {
        _led->toReset();
    } else
    {
        _led->toSet();
    }
}
void Led::_closeLed()
{
    if (LowActive == _active_level)
    {
        _led->toSet();
    } else
    {
        _led->toReset();
    }
}

void Led::run()
{
    switch (_pattern)
    {
        case LightPattern::BREATHE:
            _breathe();
            break;
        case LightPattern::SLOW_FLASHING:
            _slowFlashing();
            break;
        case LightPattern::FAST_FLASHING:
            _fastFlashing();
            break;
        case LightPattern::PULSE_FLASHING:
            _pulseFlashing();
            break;
        case LightPattern::DOUBLE_PULSE_FLASHING:
            _doublePulseFlashing();
            break;
        case LightPattern::KEEP_ON:
            _keepOn();
            break;
        case LightPattern::KEEP_OFF:
            _keepOff();
            break;
        default:
            break;
    }
}

