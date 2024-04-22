//
// Created by zhaohe on 2023/8/20.
//

#ifndef GPIO_DRIVER_PARAM_H_
#define GPIO_DRIVER_PARAM_H_

#include "driver/gpio.h"
#include "type.h"

typedef uint8_t *GpioPort;
typedef gpio_num_t GpioPin;

enum GpioState
{
    GPIO_SET = 1,
    GPIO_RESET = 0,
};

struct GpioHandle
{
    GpioHandle();

    gpio_config_t config{};
    gpio_num_t pin = GPIO_NUM_NC;
    GpioState state = GPIO_RESET;
};


#endif //GPIO_DRIVER_PARAM_H_
