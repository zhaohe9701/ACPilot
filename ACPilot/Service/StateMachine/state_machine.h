//
// Created by zhaohe on 2024/4/14.
//

#ifndef STATE_MACHINE_H_
#define STATE_MACHINE_H_

#include "Type/type.h"
#include "Notify/notify.h"
#include "Queue/queue.h"
#include "Thread/thread.h"

#define STATE_NAME_LEN PARAM_NAME_LEN

namespace Service
{

    class State;

    class State
    {
    public:
        explicit State(const char *name, Event enter_action, Event leave_action);

        AC_RET addNextState(State *state, Event event);

        State *trans(Event event);

        void enterAction();

        void leaveAction();

    private:
        char _name[STATE_NAME_LEN] = {0};
        State *_next_state[EVENT_NUM] = {nullptr};
        Event _enter_action = EVENT_NUM;
        Event _leave_action = EVENT_NUM;
    };

    class StateMachine
    {
    public:
        static AC_RET init();

        static AC_RET add(State *state);

        static AC_RET setStartState(State *state);

        static AC_RET start();

        static void _loop(void *param);

    private:
        static Common::List<State *> _state_list;
        static Osal::Queue<Event> _event_queue;
        static State *_current_state;
        static Osal::AcThread *_state_machine_task;

        static void _monitor(void *arg);
    };
}
#endif //STATE_MACHINE_H_
