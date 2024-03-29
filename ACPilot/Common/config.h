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
#ifndef __CONFIG_H__
#define __CONFIG_H__
#include <stdint.h>
/*------------------------配置区------------------------*/

#define BASE_DEBUG
/*系统*/
#define SEMAPHORE_NUM       5
#define QUEUE_NUM           2
#define MUTEX_NUM           2
#define DEBUG_PORT          0x01

/*状态机*/
#define EVENT_NUM           8
#define EVENT_QUEUE_LEN     4

/*传感器*/
#define IMU1                Icm42688
#define IMU1_INTERFACE_OBJ  hspi1
#define IMU1_INTERFACE      SpiBus
#define IMU1_CS_PORT        IMU1_CS_GPIO_Port
#define IMU1_CS_PIN         IMU1_CS_Pin
#define IMU1_EXIT_PIN       SPI1_EXIT_Pin

// #define MAGNETOMETER

#define BARO                Ms5611
#define BARO_INTERFACE_OBJ  hi2c1
#define BARO_INTERFACE      Iic
#define BARO_ADDRESS        MS5611_ADDR

/*电机*/
#define MOTOR_OUTPUT_MAX    100.0f
#define MOTOR_OUTPUT_MIN    0.0f
#define MOTOR_NUM           4
/*舵机*/
#define SERVO_NUM           0
#define MOTOR_1_TIM                 htim8
#define MOTOR_1_CHANNEL         TIM_CHANNEL_1

#define MOTOR_2_TIM                 htim8
#define MOTOR_2_CHANNEL         TIM_CHANNEL_2

#define MOTOR_3_TIM                 htim8
#define MOTOR_3_CHANNEL         TIM_CHANNEL_3

#define MOTOR_4_TIM                 htim8
#define MOTOR_4_CHANNEL         TIM_CHANNEL_4

/*控制*/
#define ATTITUDE_CONTROLLER_PARAM_NUM   36
#define ACTUAL_STATE_MUTEX              mutex[0]
#define EXPECT_STATE_MUTEX              mutex[1]
#define ACTUAL_STATE_SEMAPHORE          semaphore[1]
/*通信*/
#define RECEIVE_MESSAGE_QUEUE           queue[0]
#define TRANSMIT_MESSAGE_QUEUE          queue[1]
#define MESSAGE_TRANSMIT_QUEUE_LEN  10
#define MESSAGE_RECEIVE_QUEUE_LEN   5
#define MESSAGE_QUEUE_MAX_LENGTH    10
#define MAX_MESSAGE_PACKAGE_LENGTH  64
#define MAX_MESSAGE_BUF_LEN         1024
#define USB_PORT_NUMBER             0x01
#define UART_PORT_NUMBER            0x02

/*遥控*/
#define MAX_CHANNEL_NUM             8

enum ChannelMap
{
    MODE_CHANNEL,
    LOCK_CHANNEL,
    PITCH_CHANNEL,
    ROLL_CHANNEL,
    YAW_CHANNEL,
    THROTTLE_CHANNEL,
    X_CHANNEL,
    Y_CHANNEL,
    Z_CHANNEL,
    CHANNEL_MAP_LEN
};
#define MANUAL_SWITCH               IBUS_SWITCH_1
#define ALTITUDE_SWITCH             IBUS_SWITCH_0
#define AUTO_SWITCH                 IBUS_SWITCH_2

#define LOCK_SWITCH                 IBUS_SWITCH_0
#define UNLOCK_SWITCH               IBUS_SWITCH_2

#define REMOTE_ATTITUDE_SCOPE   30.0f
#define REMOTE_POSITION_SCOPE   30.0f
#define REMOTE_THROTTLE_SCOPE   100.0f
#define REMOTE_PALSTANCE_SCOPE  60.0f

/*消息*/


/*循环频率*/
#define ATTITUDE_SOLVE_CYCLE_FREQ   500
#define ATTITUDE_CONTROL_CYCLE_FREQ 500
#define POSITION_CONTROL_CYCLE_FREQ 50
#define LIGHT_CONTROL_CYCLE_FREQ    1000
#define DATA_LINK_CYCLE_FREQ        50
#define BARO_UPDATE_CYCLE_FREQ      20
/*------------------------------------------------------*/


#endif