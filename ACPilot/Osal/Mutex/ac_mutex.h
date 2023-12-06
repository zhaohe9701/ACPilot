//
// Created by zhaohe on 2023/8/23.
//

#ifndef AC_MUTEX_H_
#define AC_MUTEX_H_
#include "type.h"
#include "os.h"

class AcMutex
{
public:
    AcMutex();
    AC_RET lock(uint32_t timeout = AC_FOREVER);
    void unlock();
private:
    SemaphoreHandle_t _handle = nullptr;
};
#endif //AC_MUTEX_H_
