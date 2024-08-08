//
// Created by zhaohe on 2024/4/11.
//

#ifndef NOTIFY_H_
#define NOTIFY_H_

#include "Type/type.h"
#include "event_list.h"
#include "List/ac_list.h"
#include "Semaphore/semaphore.h"

namespace Utils
{

class NotifyToken
{
    friend class Notify;

public:
    AC_RET wait(uint32_t timeout = AC_FOREVER);

    Event getEvent();

protected:
    Osal::BinSemaphore _semaphore;
    Event _event = EVENT_NUM;
    TaskFunction _func = nullptr;
    void *_param = nullptr;

    void _callback();

    void _give();
};

class Notify
{
public:
    static NotifyToken *sub(Event event, TaskFunction func = nullptr, void *param = nullptr);

    static void notify(Event event);

private:
    static Common::List<NotifyToken *> _list[EVENT_NUM];
};

}

#endif //NOTIFY_H_
