//
// Created by zhaohe on 2024/4/19.
//

#include "light_server.h"
#include "Debug/default_debug.h"

using namespace Service;

Common::List<Light *> LightServer::_list;
Osal::AcThread *LightServer::_light_task = nullptr;
Utils::Mailbox<LightMessage> *LightServer::_mailbox = nullptr;

AC_RET LightServer::init()
{
    _mailbox = Utils::Mailbox<LightMessage>::find("light");
    if (nullptr == _mailbox)
    {
        BASE_ERROR("object can't find");
        return AC_ERROR;
    }
    _light_task = new Osal::AcThread("light", LIGHT_TASK_STACK_SIZE, LIGHT_TASK_PRIO, LIGHT_TASK_CORE);
    return AC_OK;
}

AC_RET LightServer::start()
{
    if (nullptr == _light_task)
    {
        BASE_ERROR("service not init");
        return AC_ERROR;
    }
    _light_task->run(_loop, nullptr);
    return AC_OK;
}

void LightServer::add(Light *light)
{
    _list.pushBack(light);
}

void LightServer::_loop(void *param)
{
    if (nullptr == _mailbox)
    {
        BASE_ERROR("NULL ptr");
        Osal::AcThread::killSelf();
    }
    for (;;)
    {
        LightMessage msg;
        if (AC_OK != _mailbox->pop(&msg))
        {
            continue;
        }
        for (Common::ListNode<Light *> *it = _list.begin(); it != _list.end(); it = it->getNext())
        {
            if ((*(*it))->match(msg.id))
            {
                (*(*it))->setMode(msg.mode);
                break;
            }
        }
    }
}

