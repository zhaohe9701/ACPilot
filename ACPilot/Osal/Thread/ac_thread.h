//
// Created by zhaohe on 2023/5/9.
//

#ifndef AC_THREAD_H_
#define AC_THREAD_H_

#include "os.h"
#include "type.h"

#define TaskFunction TaskFunction_t

class AcThread
{
public:
    AcThread(const char *name, uint16_t stack, AcPriority prio);
    void suspend();
    AC_RET resume();
    void kill();
    AC_RET run(TaskFunction func, void *param);
    void static killSelf();
protected:
    ThreadHandle _handle = nullptr;
    char _name[PARAM_NAME_LEN] = {0};
    uint16_t _stack_size = 0;
    AcPriority _prio = 0;
};


#endif //AC_THREAD_H_
