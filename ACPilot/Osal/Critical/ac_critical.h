//
// Created by zhaohe on 2023/8/4.
//

#ifndef AC_CRITICAL_H_
#define AC_CRITICAL_H_
#include "os.h"

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
#endif //AC_CRITICAL_H_
