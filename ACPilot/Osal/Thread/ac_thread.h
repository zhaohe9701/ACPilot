//
// Created by zhaohe on 2023/5/9.
//

#ifndef AC_THREAD_H_
#define AC_THREAD_H_

#include "os.h"
#include "type.h"
#include "ac_list.h"

#define THREAD_NAME_LEN PARAM_NAME_LEN

class AcThread
{
public:
    AcThread(const char *name, uint16_t stack, AcPriority prio);

    void suspend();

    AC_RET resume();

    void kill();

    AC_RET run(TaskFunction func, void *param);

    bool match(const char *name);

    uint32_t static getInfo(char *buf, uint32_t len);

    void static killSelf();

//    static AcThread *find(char *name);

private:
    ThreadHandle _handle = nullptr;
    char _name[THREAD_NAME_LEN] = {0};
    uint16_t _stack_size = 0;
    AcPriority _prio = 0;
//    static List<AcThread *> _list;
};


#endif //AC_THREAD_H_
