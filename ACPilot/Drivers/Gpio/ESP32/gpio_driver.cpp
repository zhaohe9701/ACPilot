//
// Created by zhaohe on 2023/8/20.
//
#include <string.h>
#include "Gpio/gpio_driver.h"

using namespace Driver;

GpioHandle::GpioHandle()
{
    memset(&config, 0, sizeof(gpio_config_t));
}

Gpio::Gpio(GpioHandle *handle)
{
    _handle = handle;
}

AC_RET Gpio::init()
{
    if (ESP_OK != gpio_config(&_handle->config))
    {
        return AC_ERROR;
    }
    if (ESP_OK != gpio_set_level(_handle->pin, _handle->state))
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
    return _handle->pin;
}

AC_RET Gpio::toSet()
{
    if (ESP_OK != gpio_set_level(_handle->pin, GPIO_SET))
    {
        return AC_ERROR;
    }
    return AC_OK;
}

AC_RET Gpio::toReset()
{
    if (ESP_OK != gpio_set_level(_handle->pin, GPIO_RESET))
    {
        return AC_ERROR;
    }
    return AC_OK;
}

AC_RET Gpio::flip()
{
    if (GPIO_SET == gpio_get_level(_handle->pin))
    {
        if (ESP_OK != gpio_set_level(_handle->pin, GPIO_RESET))
        {
            return AC_ERROR;
        }
    } else
    {
        if (ESP_OK != gpio_set_level(_handle->pin, GPIO_SET))
        {
            return AC_ERROR;
        }
    }
    return AC_OK;
}

AC_RET Gpio::set(GpioState state)
{
    if (ESP_OK != gpio_set_level(_handle->pin, state))
    {
        return AC_ERROR;
    }
    return AC_OK;
}



