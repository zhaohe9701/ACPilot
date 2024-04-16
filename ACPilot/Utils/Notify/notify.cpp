//
// Created by zhaohe on 2024/4/11.
//

#include "notify.h"

AC_RET NotifyToken::wait(uint32_t timeout)
{
    return _semaphore.get(timeout);
}

void NotifyToken::_callback()
{
    if (_func == nullptr)
    {
        return;
    }
    _func(_param);
}

void NotifyToken::_give()
{
    _semaphore.give();
}

Event NotifyToken::getEvent()
{
    return _event;
}

NotifyToken *Notify::sub(Event event, TaskFunction func, void *param)
{
    if (event >= EVENT_NUM)
    {
        return nullptr;
    }
    NotifyToken *token = new NotifyToken();

    token->_func = func;
    token->_event = event;
    if (param == nullptr)
    {
        token->_param = token;
    } else
    {
        token->_param = param;
    }
    _list[event].pushBack(token);

    return token;
}

List<NotifyToken *> Notify::_list[EVENT_NUM];

void Notify::notify(Event event)
{
    for (ListNode<NotifyToken *> *it = _list[event].begin(); it != _list[event].end(); it = it->getNext())
    {
        (**it)->_callback();
        (**it)->_give();
    }
}
