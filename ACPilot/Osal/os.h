/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-10-23 00:05:07
 * @LastEditors: zhaohe
 * @LastEditTime: 2023-03-02 00:01:37
 * @FilePath: \ZH_FLIGHT\Sys\OS\os.h
 * Copyright (C) 2022 zhaohe. All rights reserved.
 */
#ifndef OS_H_
#define OS_H_

#ifdef C_STM32
#include "FreeRTOS.h"
#include "queue.h"
#include "semphr.h"
#include "event_groups.h"
#endif

#ifdef C_ESP32
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"
#include "freertos/event_groups.h"
#include "freertos/task.h"
#endif

inline int is_isr()
{
    // uint32_t ipsr;
    // __asm volatile ("mrs %0, IPSR" : "=r" (ipsr) );
    // return (ipsr != 0);
    return 0;
}

typedef TaskHandle_t ThreadHandle;
typedef SemaphoreHandle_t SemaphoreHandle;
typedef UBaseType_t AcPriority;
typedef TickType_t Tick;
#define AC_FOREVER      0xFFFFFFFF
#define AC_IMMEDIATELY  0x0

#define IS_IN_IRQ() (is_isr() != 0)

#define tickSleep(tick) vTaskDelay((tick))
#define getTick() xTaskGetTickCount()
#define msToTick(ms) (pdMS_TO_TICKS(ms))
#endif
