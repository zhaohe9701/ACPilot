//
// Created by zhaohe on 2023/8/17.
//
#ifndef INTERRUPT_DRIVER_PARAM_H_
#define INTERRUPT_DRIVER_PARAM_H_

#include "Gpio/gpio_driver.h"

struct ExtInterruptHandle
{
    gpio_config_t config{};
    gpio_num_t pin = GPIO_NUM_NC;
    uint32_t priority = 0;
};

#endif //INTERRUPT_DRIVER_PARAM_H_
