//
// Created by zhaohe on 2024/4/19.
//

#include "led.h"
#include "default_debug.h"

Led::Led(Gpio *pin, GpioState on, LightMode mode, uint8_t id) : Light(id)
{
    char name[PARAM_NAME_LEN] = {0};
    snprintf(name, PARAM_NAME_LEN - 1, "led%d", _id);
    _pin = pin;
    _mode = mode;
    if (GPIO_SET == on)
    {
        _on = GPIO_SET;
        _off = GPIO_RESET;
    } else
    {
        _on = GPIO_RESET;
        _off = GPIO_SET;
    }
    _thread = new AcThread(name, LED_TASK_STACK_SIZE, LED_TASK_PRIO, LED_TASK_CORE);
    _thread->run(_loop, this);
}

AC_RET Led::setMode(LightMode mode)
{
    _mode = mode;
    return AC_OK;
}

static const uint8_t keep_on[] = {10};
static const uint8_t keep_off[] = {0};
static const uint8_t fast_flashing[] = {10, 10, 0, 0};
static const uint8_t slow_flashing[] = {10, 10, 0, 0};
static const uint8_t breathe[] = {0, 0, 0, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1};
static const uint8_t pulse_flashing[] = {10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
static const uint8_t double_pulse_flashing[] = {10, 0, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

static void control(Led *led, const uint8_t *l, uint16_t len, uint16_t &i, uint16_t time)
{
    if (i >= len)
    {
        i = 0;
    } else
    {
        led->setBright(l[i++], time);
    }
}

void Led::_loop(void *param)
{
    Led *led = static_cast<Led *>(param);
    uint16_t i = 0;
    for (;;)
    {
        switch (led->_mode)
        {
            case LIGHT_KEEP_OFF:
                control(led, keep_off, sizeof(keep_off), i, 10);
                break;
            case LIGHT_KEEP_ON:
                control(led, keep_on, sizeof(keep_on), i, 10);
                break;
            case LIGHT_FAST_FLASHING:
                control(led, fast_flashing, sizeof(fast_flashing), i, 10);
                break;
            case LIGHT_SLOW_FLASHING:
                control(led, slow_flashing, sizeof(slow_flashing), i, 40);
                break;
            case LIGHT_BREATHE:
                control(led, breathe, sizeof(breathe), i, 10);
                break;
            case LIGHT_PULSE_FLASHING:
                control(led, pulse_flashing, sizeof(pulse_flashing), i, 10);
                break;
            case LIGHT_DOUBLE_PULSE_FLASHING:
                control(led, double_pulse_flashing, sizeof(double_pulse_flashing), i, 10);
                break;
            default:
                BASE_ERROR("not support light mode:%d", led->_mode);
                break;
        }
    }
}

void Led::setBright(uint8_t bright, uint16_t time)
{
    for (int i = 0; i < time; i++)
    {
        if (bright != 0)
        {
            _pin->set(_on);
        }
        for (int j = 0; j < 10; j++)
        {
            if (j == bright)
            {
                _pin->set(_off);
            }
            tickSleep(1);
        }
    }
}

