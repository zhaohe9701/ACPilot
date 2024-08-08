//
// Created by zhaohe on 2023/8/4.
//

#ifndef CRITICAL_H_
#define CRITICAL_H_
#include "os.h"

namespace Osal
{
    class Critical
    {
    public:
        static void enter();

        static void exit();

    private:
#ifdef C_ESP32
        static portMUX_TYPE _spinlock;
#endif
    };
}

#endif //CRITICAL_H_
