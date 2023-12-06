//
// Created by zhaohe on 2023/8/23.
//
#include "ac_mutex.h"

AcMutex::AcMutex()
{
    _handle = xSemaphoreCreateMutex();
}
AC_RET AcMutex::lock(uint32_t timeout)
{
    if (pdTRUE != xSemaphoreTake(_handle, (TickType_t)timeout))
    {
        return AC_ERROR;
    }
    return AC_OK;
}
void AcMutex::unlock()
{
    xSemaphoreGive(_handle);
}
