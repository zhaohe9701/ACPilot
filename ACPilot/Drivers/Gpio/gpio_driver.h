//
// Created by zhaohe on 2023/10/17.
//

#ifndef GPIO_DRIVER_H_
#define GPIO_DRIVER_H_

#ifdef C_STM32
#include "STM32/gpio_driver_param.h"
#endif

#ifdef C_ESP32

#include "ESP32/gpio_driver_param.h"

#endif

namespace Driver
{

    class Gpio
    {
    public:
        explicit Gpio(GpioHandle *handle);

        AC_RET init();

        GpioPort getPort();

        GpioPin getPin();

        AC_RET toSet();

        AC_RET toReset();

        AC_RET flip();

        AC_RET set(GpioState state);

    private:
        GpioHandle *_handle = nullptr;
    };
}

#endif //GPIO_DRIVER_H_
