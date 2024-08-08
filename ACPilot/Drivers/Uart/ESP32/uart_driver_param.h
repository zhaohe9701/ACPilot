/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-10-21 23:59:58
 * @LastEditors: zhaohe
 * @LastEditTime: 2023-02-27 23:14:22
 * @FilePath: \ZH_FLIGHT\Sys\Driver\z_uart.h
 * Copyright (C) 2022 zhaohe. All rights reserved.
 */
#ifndef UART_DRIVER_PARAM_H_
#define UART_DRIVER_PARAM_H_

#include <string.h>
#include "driver/uart.h"
#include "driver/gpio.h"
namespace Driver
{
    struct UartHandle
    {
        UartHandle();

        uart_config_t config{};
        uart_port_t index = UART_NUM_0;
        gpio_num_t tx = GPIO_NUM_NC;
        gpio_num_t rx = GPIO_NUM_NC;
        gpio_num_t cts = GPIO_NUM_NC;
        gpio_num_t rts = GPIO_NUM_NC;
    };
}
#endif
