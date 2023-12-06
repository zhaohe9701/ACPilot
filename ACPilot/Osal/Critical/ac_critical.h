//
// Created by zhaohe on 2023/8/4.
//

#ifndef __AC_CRITICAL_H__
#define __AC_CRITICAL_H__
#include "os.h"

class Critical
{
public:
    static void enter();
    static void exit();
};
#endif //__AC_CRITICAL_H__
