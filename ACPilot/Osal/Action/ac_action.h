//
// Created by zhaohe on 2023/7/30.
//

#ifndef AC_ACTION_H_
#define AC_ACTION_H_
#include "os.h"
#include "type.h"

enum Action : uint32_t
{
    GET_REMOTE_COMMAND  = 1 << 0,
    GET_MEMS            = 1 << 1,
    CAL_ATTITUDE        = 1 << 2,
    CTRL_ATTITUDE       = 1 << 3,
    CTRL_ALTITUDE       = 1 << 4,
    CTRL_AUTO           = 1 << 5,
    OUTPUT_TO_MOTOR     = 1 << 6,
};



typedef EventBits_t ActionGroup;

class ActionManager
{
public:
    ActionManager();
    ActionGroup wait(ActionGroup action, AC_RELATION relation, bool clear);
    ActionGroup wait(ActionGroup action);
    ActionGroup get();
    AC_RET clear(ActionGroup action);
    AC_RET give(ActionGroup action);
    ~ActionManager();
private:
    EventGroupHandle_t _handle;
};
#endif //AC_ACTION_H_
