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

class Gpio
{
public:
    Gpio(GpioPort port, GpioPin pin);
    explicit Gpio(GpioPin pin);
    AC_RET init(GpioState state);
    GpioPort getPort();
    GpioPin getPin();
    AC_RET toSet();
    AC_RET toReset();
    AC_RET flip();

    static void enable();
private:
    GpioPort _port = nullptr;
    GpioPin _pin;
};



#endif //GPIO_DRIVER_H_
