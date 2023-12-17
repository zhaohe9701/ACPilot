//
// Created by zhaohe on 2023/8/4.
//
#include "ac_critical.h"

portMUX_TYPE Critical::_spinlock = portMUX_INITIALIZER_UNLOCKED;

void Critical::enter()
{
    if (IS_IN_IRQ())
    {
         taskENTER_CRITICAL_FROM_ISR();
    } else
    {
#ifdef C_ESP32
        portENTER_CRITICAL(&_spinlock);
#else
        taskENTER_CRITICAL();
#endif
    }
}

void  Critical::exit()
{
    if (IS_IN_IRQ())
    {
        // taskEXIT_CRITICAL_FROM_ISR();
    } else
    {
        // taskEXIT_CRITICAL();
    }
}