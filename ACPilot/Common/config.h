/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2023-01-08 23:41:40
 * @LastEditors: zhaohe
 * @LastEditTime: 2023-04-09 23:53:42
 * @FilePath: \ZH_FLIGHT\Sys\Config\config.h
 * Copyright (C) 2023 zhaohe. All rights reserved.
 */
/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-07-08 01:24:46
 * @LastEditors: zhaohe
 * @LastEditTime: 2023-02-28 23:44:23
 * @FilePath: \ZH_FLIGHT\Sys\Config\config.h
 * Copyright (C) 2022 zhaohe. All rights reserved.
 */
#ifndef CONFIG_H_
#define CONFIG_H_
#include <stdint.h>
/*------------------------配置区------------------------*/

#define BASE_DEBUG
/*系统*/
#define GENERAL_MEMORY_POOL_NUM         4
#define PER_GENERAL_MEMORY_POOL_NUM     {10, 3, 3, 3}
#define PER_GENERAL_MEMORY_POOL_SIZE    {128, 256, 1024, 2048};

#define USB_PORT_ID                     0x01
#define UART0_PORT_ID                   0x02
#define UART1_PORT_ID                   0x03
#define UART2_PORT_ID                   0x04
#define UDP_PORT_ID                     0x05
#define TCP_PORT_ID                     0x06

#define DEBUG_PORT_ID                   USB_PORT_ID

#define MAX_MESSAGE_BUF_LEN             2048
#define MAX_UART_BUF_LEN                MAX_MESSAGE_BUF_LEN
#define MAX_USB_BUF_LEN                 MAX_MESSAGE_BUF_LEN
#define MAX_UDP_BUF_LEN                 MAX_MESSAGE_BUF_LEN


/*遥控*/
#define MAX_CHANNEL_NUM                 8

#define RECEIVE_TASK_CORE               1
#define RECEIVE_TASK_PRIO               11
#define RECEIVE_TASK_STACK_SIZE         2500

#define COMMAND_TASK_CORE               1
#define COMMAND_TASK_PRIO               9
#define COMMAND_TASK_STACK_SIZE         3000

#define LIGHT_TASK_CORE                 1
#define LIGHT_TASK_PRIO                 2
#define LIGHT_TASK_STACK_SIZE           2000

#define FSM_TASK_CORE                   1
#define FSM_TASK_PRIO                   22
#define FSM_TASK_STACK_SIZE             2000

#define SEND_TASK_CORE                  1
#define SEND_TASK_PRIO                  10
#define SEND_TASK_STACK_SIZE            2000

#define LED_TASK_CORE                   1
#define LED_TASK_PRIO                   3
#define LED_TASK_STACK_SIZE             2000

#define UART_RECEIVE_TASK_CORE          1
#define UART_RECEIVE_TASK_PRIO          21
#define UART_RECEIVE_TASK_STACK         2000

#define USB_RECEIVE_TASK_CORE           1
#define USB_RECEIVE_TASK_PRIO           21
#define USB_RECEIVE_TASK_STACK          2000

#define UDP_RECEIVE_TASK_CORE           1
#define UDP_RECEIVE_TASK_PRIO           23
#define UDP_RECEIVE_TASK_STACK          3000

#define MAIN_CONTROL_TASK_CORE          0
#define MAIN_CONTROL_TASK_PRIO          23
#define MAIN_CONTROL_TASK_STACK_SIZE    4000

#define POS_CONTROL_TASK_CORE           0
#define POS_CONTROL_TASK_PRIO           22
#define POS_CONTROL_TASK_STACK_SIZE     4000

#define CALI_TASK_CORE                  0
#define CALI_TASK_PRIO                  10
#define CALI_TASK_STACK_SIZE            4000

#define BARO_TASK_CORE                  0
#define BARO_TASK_PRIO                  22
#define BARO_TASK_STACK_SIZE            2500

#define MONITOR_TASK_CORE               1
#define MONITOR_TASK_PRIO               9
#define MONITOR_TASK_STACK_SIZE         2000
/*------------------------------------------------------*/


#endif