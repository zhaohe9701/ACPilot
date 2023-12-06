#include "advanced_thread.h"

static void mainLoop(void *param)
{
    ThreadParam *_param = static_cast<ThreadParam *>(param);
    STEP_RET ret = STEP_CONTINUE;
    if (nullptr != param)
    {
        _param->start();
    }
    while (true)
    {
        ActionGroup action = 0x0;
        if (0x0 != _param->trigger)
        {
             action = _param->action_manager->wait(_param->trigger);
        }
        for (ListNode<LoopFunc> *it = _param->loop.begin(); it != _param->loop.end(); it = it->getNext())
        {
            if (STEP_CONTINUE != (ret = (*(*it))(action)))
            {
                break;
            }
        }
        if (STEP_CONTINUE != ret)
        {
            break;
        }
    }
    if (STEP_ERROR == ret && nullptr != _param->err)
    {
        _param->err();
        return;
    }
    if (STEP_END == ret && nullptr != _param->end)
    {
        _param->end();
        return;
    }
}

AdvancedThread::AdvancedThread(const char *name, ActionGroup trigger, uint16_t stack, AcPriority prio) : AcThread(name, stack, prio)
{
    _param.trigger = trigger;
}

AC_RET AdvancedThread::add(FUNC_TYPE type, LoopFunc func)
{
    if (LOOP_FUNC != type)
    {
        return AC_ERROR;
    }
    _param.loop.pushBack(func);
    return AC_OK;
}

AC_RET AdvancedThread::add(FUNC_TYPE type, EndsFunc func)
{
    if (START_FUNC == type)
    {
        _param.start = func;
        return AC_OK;
    } else if (END_FUNC == type)
    {
        _param.end = func;
        return AC_OK;
    } else if (ERR_FUNC == type)
    {
        _param.err = func;
        return AC_OK;
    } else
    {
        return AC_ERROR;
    }
}

AC_RET AdvancedThread::addActionManager(ActionManager *manager)
{
    if (nullptr == manager)
    {
        return AC_ERROR;
    }
    _param.action_manager = manager;
    return AC_OK;
}


List<AdvancedThread*> ThreadManager::_list;
ActionManager *ThreadManager::_manager = nullptr;

AC_RET ThreadManager::run(AdvancedThread *thread)
{
    if (nullptr == thread)
    {
        return AC_ERROR;
    }
    _list.pushBack(thread);
    if (AC_OK != thread->addActionManager(_manager))
    {
        return AC_ERROR;
    }
    if (pdPASS != xTaskCreate(mainLoop, thread->_name, thread->_stack_size, &thread->_param, thread->_prio, &thread->_handle))
    {
        return AC_ERROR;
    }
    return AC_OK;
}

AC_RET ThreadManager::addActionManager(ActionManager *manager)
{
    if (nullptr == manager)
    {
        return AC_ERROR;
    }
    _manager = manager;
    return AC_OK;
}