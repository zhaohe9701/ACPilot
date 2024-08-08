//
// Created by zhaohe on 2023/5/9.
//

#include <string.h>
#include "thread.h"

using namespace Osal;

AcThread::AcThread(const char *name, uint16_t stack, AcPriority prio, int16_t core)
{
    strncpy(_name, name, PARAM_NAME_LEN - 1);
    _stack_size = stack;
    _prio = prio;
    _core = core;
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

AC_RET AcThread::run(TaskFunction func, void *param)
{
#ifdef C_ESP32
    if (pdPASS != xTaskCreatePinnedToCore(func, _name, _stack_size, param, _prio, &_handle, _core))
    {
        return AC_ERROR;
    }
#else
    if (pdPASS != xTaskCreate(func, _name, _stack_size, param, _prio, &_handle))
    {
        return AC_ERROR;
    }
#endif
    return AC_OK;
}

bool AcThread::match(const char *name)
{
    if (0 == strncmp(_name, name, THREAD_NAME_LEN))
    {
        return true;
    }
    return false;
}

void AcThread::killSelf()
{
    vTaskDelete(nullptr);
}

uint32_t AcThread::getInfo(char *buf, uint32_t len)
{
    TaskStatus_t *status_array = nullptr;
    uint32_t array_size = 0;
    uint32_t total_run_time = 0;
    uint32_t stats_as_percentage = 0;
    uint32_t ptr = 0;
    char core[2] = {0};

    array_size = uxTaskGetNumberOfTasks();

    status_array = new TaskStatus_t[array_size];

    if (nullptr != status_array)
    {
        array_size = uxTaskGetSystemState(status_array, array_size, &total_run_time);

        total_run_time /= 100UL;

        for (int i = 0; i < array_size; i++)
        {

            if (total_run_time > 0)
            {
                stats_as_percentage = status_array[i].ulRunTimeCounter / total_run_time;
            } else
            {
                stats_as_percentage = 0;
            }
            if (status_array[i].xCoreID == 0)
            {
                core[0] = '0';
            } else if (status_array[i].xCoreID == 1)
            {
                core[0] = '1';
            } else
            {
                core[0] = '?';
            }

            ptr += snprintf(buf + ptr, len - ptr, "\t%-12s%-6u%-8lu%-6lu%-6s\n",
                            status_array[i].pcTaskName,
                            status_array[i].uxCurrentPriority,
                            status_array[i].usStackHighWaterMark,
                            stats_as_percentage,
                            core);
        }

        delete[] status_array;
    }

    return ptr;
}

//AcThread *AcThread::find(char *name)
//{
//    for (ListNode<AcThread *> *it = _list.begin(); it != _list.end(); it = it->getNext())
//    {
//        if ((**it)->match(name))
//        {
//            return (**it);
//        }
//    }
//    return nullptr;
//}

