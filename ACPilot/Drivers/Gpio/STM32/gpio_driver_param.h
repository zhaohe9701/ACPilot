//
// Created by zhaohe on 2023/8/20.
//

#ifndef GPIO_DRIVER_PARAM_H_
#define GPIO_DRIVER_PARAM_H_

#include "gpio.h"
#include "type.h"

typedef GPIO_TypeDef*   GpioPort;
typedef uint16_t        GpioPin;

enum GpioState
{
    GPIO_SET = GPIO_PIN_SET,
    GPIO_RESET = GPIO_PIN_RESET,
    GPIO_NONE = -1,
};

#endif //GPIO_DRIVER_PARAM_H_
