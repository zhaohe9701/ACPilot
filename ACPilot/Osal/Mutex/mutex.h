//
// Created by zhaohe on 2023/8/23.
//

#ifndef MUTEX_H_
#define MUTEX_H_
#include "Type/type.h"
#include "os.h"

namespace Osal
{
    class Mutex
    {
    public:
        Mutex();

        AC_RET lock(uint32_t timeout = AC_FOREVER);

        void unlock();

    private:
        SemaphoreHandle_t _handle = nullptr;
    };
}
#endif //MUTEX_H_
