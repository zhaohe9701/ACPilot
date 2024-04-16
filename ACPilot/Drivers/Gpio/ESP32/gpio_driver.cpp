//
// Created by zhaohe on 2023/8/20.
//
#include "Gpio/gpio_driver.h"

Gpio::Gpio(GpioPort port, GpioPin pin)
{
    _pin = pin;
}

Gpio::Gpio(GpioPin pin)
{
    _pin = pin;
}

AC_RET Gpio::init(GpioState state)
{
    if (ESP_OK != gpio_set_level(_pin, GPIO_SET))
    {
        return AC_ERROR;
    }
    return AC_OK;
}

GpioPort Gpio::getPort()
{
    return nullptr;
}

GpioPin Gpio::getPin()
{
    return _pin;
}

AC_RET Gpio::toSet()
{
    if (ESP_OK != gpio_set_level(_pin, GPIO_SET))
    {
        return AC_ERROR;
    }
    return AC_OK;
}

AC_RET Gpio::toReset()
{
    if (ESP_OK != gpio_set_level(_pin, GPIO_RESET))
    {
        return AC_ERROR;
    }
    return AC_OK;
}

AC_RET Gpio::flip()
{
    return AC_ERROR;
}

void Gpio::enable() {}


