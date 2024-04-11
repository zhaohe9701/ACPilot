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

NotifyToken *Notify::sub(Event event, TaskFunction func, void *param)
{
    if (event >= EVENT_NUM)
    {
        return nullptr;
    }

    NotifyToken *token = new NotifyToken();

    token->_func = func;
    token->_param = param;
    token->_event = event;
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
