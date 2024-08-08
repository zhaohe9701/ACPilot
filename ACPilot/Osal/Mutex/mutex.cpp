//
// Created by zhaohe on 2023/8/23.
//
#include "mutex.h"

using namespace Osal;

Mutex::Mutex()
{
    _handle = xSemaphoreCreateMutex();
}
AC_RET Mutex::lock(uint32_t timeout)
{
    if (pdTRUE != xSemaphoreTake(_handle, (TickType_t)timeout))
    {
        return AC_ERROR;
    }
    return AC_OK;
}
void Mutex::unlock()
{
    xSemaphoreGive(_handle);
}
