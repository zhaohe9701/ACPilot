#ifndef ADVANCED_THREAD_
#define ADVANCED_THREAD_

#include "Thread/ac_thread.h"
#include "List/ac_list.h"
#include "Action/ac_action.h"

enum STEP_RET
{
    STEP_CONTINUE,
    STEP_END,
    STEP_ERROR,
};

enum FUNC_TYPE
{
    START_FUNC,
    LOOP_FUNC,
    END_FUNC,
    ERR_FUNC
};
typedef STEP_RET (*LoopFunc)(ActionGroup action);
typedef STEP_RET (*EndsFunc)();

class ThreadManager;

struct ThreadParam
{
    ActionManager *action_manager = nullptr;
    ActionGroup trigger = 0x0;
    EndsFunc start = nullptr;
    List<LoopFunc> loop;
    EndsFunc end = nullptr;
    EndsFunc err = nullptr;
};

class AdvancedThread : public AcThread
{
    friend ThreadManager;
public:
    AdvancedThread(const char *name, ActionGroup trigger, uint16_t stack, AcPriority prio);
    AC_RET add(FUNC_TYPE type, LoopFunc func);
    AC_RET add(FUNC_TYPE type, EndsFunc func);
private:
    AC_RET addActionManager(ActionManager *manager);
    ThreadParam _param;
};

class ThreadManager
{
public:
    static AC_RET run(AdvancedThread *thread);
    static AC_RET addActionManager(ActionManager *manager);
private:
    static List<AdvancedThread*> _list;
    static ActionManager *_manager;
};


#endif