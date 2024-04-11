/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2023-03-26 20:02:09
 * @LastEditors: zhaohe
 * @LastEditTime: 2023-03-26 21:21:10
 * @FilePath: \ZH_FLIGHT\Sys\OS\ac_queue.h
 * Copyright (C) 2023 zhaohe. All rights reserved.
 */
#ifndef AC_QUEUE_H_
#define AC_QUEUE_H_

#include "os.h"
#include "type.h"

typedef QueueHandle_t QueueHandle;

template <class T>
class AcQueue
{
public:
    explicit AcQueue(QueueHandle handle);
    explicit AcQueue(uint32_t len);
    AC_RET push(T *data, uint32_t timeout = AC_FOREVER);
    AC_RET pop(T *data, uint32_t timeout = AC_FOREVER);
    void reset();
    uint32_t getNum();
    QueueHandle getHandle();
    ~AcQueue();
private:
    QueueHandle _handler = nullptr;
};

template<class T>
AcQueue<T>::AcQueue(QueueHandle handle)
{
    _handler = handle;
}


template<class T>
uint32_t AcQueue<T>::getNum()
{
    if (IS_IN_IRQ())
    {
        return uxQueueMessagesWaitingFromISR( _handler);
    } else
    {
        return uxQueueMessagesWaiting(_handler);
    }
}

template<class T>
QueueHandle AcQueue<T>::getHandle()
{
    return _handler;
}

template <class T>
AcQueue<T>::AcQueue(uint32_t len)
{
    _handler = xQueueCreate(len, sizeof(T));
}

template <class T>
AC_RET AcQueue<T>::push(T *data, uint32_t timeout)
{
    if (IS_IN_IRQ())
    {
        BaseType_t yield = pdFALSE;
        if (pdTRUE != xQueueSendToBackFromISR(_handler, data, &yield))
        {
            return AC_ERROR;
        } else
        {
            portYIELD_FROM_ISR(yield);
        }
    } else
    {
        if (pdTRUE != xQueueSendToBack(_handler, data, (TickType_t)timeout))
        {
            return AC_ERROR;
        }
    }
    return AC_OK;
}

template <class T>
AC_RET AcQueue<T>::pop(T *data, uint32_t timeout)
{
    if (IS_IN_IRQ())
    {
        BaseType_t yield = pdFALSE;
        if (pdTRUE != xQueueReceiveFromISR(_handler, data, &yield))
        {
            return AC_ERROR;
        } else
        {
            portYIELD_FROM_ISR(yield);
        }
    } else
    {
        if (pdTRUE != xQueueReceive(_handler, data, (TickType_t)timeout))
        {
            return AC_ERROR;
        }
    }
    return AC_OK;
}

template<class T>
void AcQueue<T>::reset()
{
    xQueueReset(_handler);
}

template <class T>
AcQueue<T>::~AcQueue()
{
    vQueueDelete(_handler);
}

#endif