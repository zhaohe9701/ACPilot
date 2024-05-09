//
// Created by zhaohe on 2024/4/14.
//
#include <string.h>
#include "state_machine.h"
#include "default_debug.h"

State::State(const char *name, Event enter_action, Event leave_action)
{
    strncpy(_name, name, sizeof(_name));
    _enter_action = enter_action;
    _leave_action = leave_action;
    StateMachine::add(this);
}

AC_RET State::addNextState(State *state, Event event)
{
    if (nullptr == state)
    {
        return AC_ERROR;
    }
    _next_state[event] = state;
    return AC_OK;
}

State *State::trans(Event event)
{
    return _next_state[event];
}

void State::enterAction()
{
    Notify::notify(_enter_action);
}

void State::leaveAction()
{
    Notify::notify(_leave_action);
}

List<State *> StateMachine::_state_list;
AcQueue<Event> StateMachine::_event_queue{5};
AcThread *StateMachine::_state_machine_task = nullptr;
State *StateMachine::_current_state = nullptr;

AC_RET StateMachine::init()
{
    /* 监听所有事件 */
    for (int i = 0; i < EVENT_NUM; ++i)
    {
        Notify::sub((Event)i, _monitor);
    }

    _state_machine_task = new AcThread("fsm", FSM_TASK_STACK_SIZE, FSM_TASK_PRIO, FSM_TASK_CORE);
    return AC_OK;
}

AC_RET StateMachine::add(State *state)
{
    if (nullptr == state)
    {
        return AC_ERROR;
    }
    _state_list.pushBack(state);
    return AC_OK;
}

AC_RET StateMachine::setStartState(State *state)
{
    if (nullptr == state)
    {
        return AC_ERROR;
    }
    _current_state = state;
    return AC_OK;
}

AC_RET StateMachine::start()
{
    if (nullptr == _current_state)
    {
        _current_state = **_state_list.begin();
    }
    _state_machine_task->run(_loop, nullptr);
    return AC_OK;
}

void StateMachine::_monitor(void *arg)
{
    NotifyToken *token = (NotifyToken *) arg;
    if (nullptr == token)
    {
        return;
    }
    Event event = token->getEvent();

    _event_queue.push(&event);
}

void StateMachine::_loop(void *param)
{
    if (nullptr == _current_state)
    {
        BASE_ERROR("NULL ptr");
        AcThread::killSelf();
    }
    for (;;)
    {
        Event event = EVENT_NUM;
        State *next_state = nullptr;

        if (AC_OK != _event_queue.pop(&event))
        {
            continue;
        }
        next_state = _current_state->trans(event);
        if (nullptr == next_state)
        {
            continue;
        }
        _current_state->leaveAction();
        _current_state = next_state;
        _current_state->enterAction();
    }
}


