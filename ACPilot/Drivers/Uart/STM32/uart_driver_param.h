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

#include "List/ac_list.h"
#include "Semaphore/ac_semaphore.h"
#include "Interface/com_interface.h"

typedef UART_HandleTypeDef UartHandle;
typedef USART_TypeDef UartSelect;

enum UartWordLen
{
    UART_WORDLEN_7 = UART_WORDLENGTH_7B,
    UART_WORDLEN_8 = UART_WORDLENGTH_8B,
    UART_WORDLEN_9 = UART_WORDLENGTH_9B
};

enum UartStopBits
{
    UART_STOPBIT_05 = UART_STOPBITS_0_5,
    UART_STOPBIT_10 = UART_STOPBITS_1,
    UART_STOPBIT_15 = UART_STOPBITS_1_5,
    UART_STOPBIT_20 = UART_STOPBITS_2
};

enum UartParity
{
    UART_PARITY_N = UART_PARITY_NONE,
    UART_PARITY_E = UART_PARITY_EVEN,
    UART_PARITY_O = UART_PARITY_ODD
};

enum UartHwCtrl
{
    UART_HW_CTRL_ENABLE = UART_HWCONTROL_RTS_CTS,
    UART_HW_CTRL_DISABLE = UART_HWCONTROL_NONE
};

#endif
