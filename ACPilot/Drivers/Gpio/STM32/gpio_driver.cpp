//
// Created by zhaohe on 2023/8/20.
//
#include "Gpio/gpio_driver.h"

Gpio::Gpio(GpioPort port, GpioPin pin)
{
    _port = port;
    _pin = pin;
}

Gpio::Gpio(GpioPin pin)
{
    return;
}

AC_RET Gpio::init(GpioState state)
{
    HAL_GPIO_WritePin(_port, _pin, state);
    return AC_OK;
}

GpioPort Gpio::getPort()
{
    return _port;
}

GpioPin Gpio::getPin()
{
    return _pin;
}

AC_RET Gpio::toSet()
{
    HAL_GPIO_WritePin(_port, _pin, GPIO_SET);
    return AC_OK;
}

AC_RET Gpio::toReset()
{
    HAL_GPIO_WritePin(_port, _pin, GPIO_RESET);
    return AC_OK;
}

AC_RET Gpio::flip()
{
    HAL_GPIO_TogglePin(_port, _pin);
    return AC_OK;
}

void Gpio::enable()
{
    __HAL_RCC_GPIOH_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_GPIOE_CLK_ENABLE();
}


