//
// Created by zhaohe on 2023/8/4.
//
#include "ac_critical.h"

void Critical::enter()
{
    if (IS_IN_IRQ())
    {
        // taskENTER_CRITICAL_FROM_ISR();
    } else
    {
        // taskENTER_CRITICAL();
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