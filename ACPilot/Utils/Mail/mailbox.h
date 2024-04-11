/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2023-02-27 23:27:49
 * @LastEditors: zhaohe
 * @LastEditTime: 2023-03-22 00:41:25
 * @FilePath: \ZH_FLIGHT\Sys\Data\data_manager.h
 * Copyright (C) 2023 zhaohe. All rights reserved.
 */
#ifndef MAILBOX_H_
#define MAILBOX_H_

#include <string.h>
#include "Queue/ac_queue.h"
#include "type.h"
#include "ac_list.h"

#define BOX_NAME_LEN PARAM_NAME_LEN

template<typename T>
class Mailbox;

/* ---------------------- MailboxCore ------------------------- */
class MailboxCore
{
public:
    explicit MailboxCore(const char *name, uint32_t len);

    bool match(const char *name);

    char *getName();

    uint32_t getLen();

    static List<MailboxCore *> *getList();

protected:
    static List<MailboxCore *> _list;
    char _name[BOX_NAME_LEN] = {0};
    uint32_t _len = 0;
};

/* ------------------------- Mailbox ---------------------------- */
template<class T>
class Mailbox : public MailboxCore
{
public:
    explicit Mailbox(const char *name, uint32_t len = 1);

    AC_RET push(T *data, uint32_t timeout = AC_FOREVER);

    AC_RET pop(T *data, uint32_t timeout = AC_FOREVER);

    AC_RET update(T *data);

    AC_RET copy(T *data, uint32_t timeout = AC_FOREVER);

    static Mailbox<T> *find(const char *name);

    static void add(MailboxCore *core);


    ~Mailbox();

private:
    QueueHandle _handler = nullptr;

};


template<class T>
Mailbox<T>::Mailbox(const char *name, uint32_t len) : MailboxCore(name, len)
{
    _handler = xQueueCreate(len, sizeof(T));
    Mailbox<T>::add(this);
}

template<class T>
AC_RET Mailbox<T>::push(T *data, uint32_t timeout)
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
        if (pdTRUE != xQueueSendToBack(_handler, data, (TickType_t) timeout))
        {
            return AC_ERROR;
        }
    }
    return AC_OK;
}

template<class T>
AC_RET Mailbox<T>::pop(T *data, uint32_t timeout)
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
        if (pdTRUE != xQueueReceive(_handler, data, (TickType_t) timeout))
        {
            return AC_ERROR;
        }
    }
    return AC_OK;
}

template<class T>
AC_RET Mailbox<T>::copy(T *data, uint32_t timeout)
{
    if (IS_IN_IRQ())
    {
        if (pdTRUE != xQueuePeekFromISR(_handler, data))
        {
            return AC_ERROR;
        }
    } else
    {
        if (pdTRUE != xQueuePeek(_handler, data, (TickType_t) timeout))
        {
            return AC_ERROR;
        }
    }
    return AC_OK;
}

template<class T>
AC_RET Mailbox<T>::update(T *data)
{
    if (IS_IN_IRQ())
    {
        BaseType_t yield = pdFALSE;
        if (pdTRUE != xQueueOverwriteFromISR(_handler, data, &yield))
        {
            return AC_ERROR;
        } else
        {
            portYIELD_FROM_ISR(yield);
        }
    } else
    {
        if (pdTRUE != xQueueOverwrite(_handler, data))
        {
            return AC_ERROR;
        }
    }
    return AC_OK;
}

template<typename T>
Mailbox<T> *Mailbox<T>::find(const char *name)
{
    for (ListNode<MailboxCore *> *it = _list.begin(); it != _list.end(); it = it->getNext())
    {
        if ((**it)->match(name))
        {
            return static_cast<Mailbox<T> *>(**it);
        }
    }
    return nullptr;
}


template<typename T>
void Mailbox<T>::add(MailboxCore *core)
{
    _list.pushBack(core);
}

template<class T>
Mailbox<T>::~Mailbox()
{
    vQueueDelete(_handler);
}

#endif