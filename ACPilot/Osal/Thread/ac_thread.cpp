//
// Created by zhaohe on 2023/5/9.
//

#include <string.h>
#include "ac_thread.h"


AcThread::AcThread(const char *name, uint16_t stack, AcPriority prio)
{
    strncpy(_name, name, PARAM_NAME_LEN - 1);
    _stack_size = stack;
    _prio = prio;
}

void AcThread::kill()
{
    if (nullptr != _handle)
    {
        vTaskDelete(_handle);
    }
}

void AcThread::suspend()
{
    if (nullptr != _handle)
    {
        vTaskSuspend(_handle);
    }
}

AC_RET AcThread::resume()
{
    if (nullptr != _handle)
    {
        if (IS_IN_IRQ())
        {
            if (pdPASS != xTaskResumeFromISR(_handle))
            {
                return AC_ERROR;
            } else
            {
                return AC_OK;
            }
        } else
        {
            vTaskResume(_handle);
            return AC_OK;
        }
    }
    return AC_ERROR;
}

AC_RET AcThread::run(TaskFunction_t func, void *param)
{
    if (pdPASS != xTaskCreate(func, _name, _stack_size, param, _prio, &_handle))
    {
        return AC_ERROR;
    }
    return AC_OK;
}

void AcThread::killSelf()
{
    vTaskDelete(nullptr);
}

