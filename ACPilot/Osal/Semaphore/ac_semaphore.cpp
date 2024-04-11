/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2023-03-26 22:28:07
 * @LastEditors: zhaohe
 * @LastEditTime: 2023-03-26 22:42:00
 * @FilePath: \ZH_FLIGHT\Sys\OS\ac_semaphore.cpp
 * Copyright (C) 2023 zhaohe. All rights reserved.
 */
#include "ac_semaphore.h"

AcBinSemaphore::AcBinSemaphore()
{
    _handle = xSemaphoreCreateBinary();
}

AC_RET AcBinSemaphore::give()
{
    if (IS_IN_IRQ())
    {
        BaseType_t yield = pdFALSE;
        if (pdTRUE != xSemaphoreGiveFromISR(_handle, &yield))
        {
            return AC_ERROR;
        } else
        {
            portYIELD_FROM_ISR(yield);
        }
    } else
    {
        if (pdTRUE != xSemaphoreGive(_handle))
        {
            return AC_ERROR;
        }
    }
    return AC_OK;
}

AC_RET AcBinSemaphore::get(uint32_t timeout)
{
    if (IS_IN_IRQ())
    {
        BaseType_t yield = pdFALSE;
        if (pdTRUE != xSemaphoreTakeFromISR(_handle, &yield))
        {
            return AC_ERROR;
        } else
        {
            portYIELD_FROM_ISR(yield);
        }
    } else
    {
        if (pdTRUE != xSemaphoreTake(_handle, (TickType_t)timeout))
        {
            return AC_ERROR;
        }
    }
    return AC_OK;
}

AcCountSemaphore::AcCountSemaphore(uint32_t max_count, uint32_t init_count)
{
    _handle = xSemaphoreCreateCounting(max_count, init_count);
}

AC_RET AcCountSemaphore::give()
{
    if (IS_IN_IRQ())
    {
        BaseType_t yield = pdFALSE;
        if (pdTRUE != xSemaphoreGiveFromISR(_handle, &yield))
        {
            return AC_ERROR;
        } else
        {
            portYIELD_FROM_ISR(yield);
        }
    } else
    {
        if (pdTRUE != xSemaphoreGive(_handle))
        {
            return AC_ERROR;
        }
    }
    return AC_OK;
}

AC_RET AcCountSemaphore::get(uint32_t timeout)
{
    if (IS_IN_IRQ())
    {
        BaseType_t yield = pdFALSE;
        if (pdTRUE != xSemaphoreTakeFromISR(_handle, &yield))
        {
            return AC_ERROR;
        } else
        {
            portYIELD_FROM_ISR(yield);
        }
    } else
    {
        if (pdTRUE != xSemaphoreTake(_handle, (TickType_t)timeout))
        {
            return AC_ERROR;
        }
    }
    return AC_OK;
}