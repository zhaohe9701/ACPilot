//
// Created by zhaohe on 2024/4/2.
//

#include "thread_framework.h"

AC_RET ThreadFramework::info(char *buf, uint32_t len)
{
    uint32_t ptr = 0;
    ptr += snprintf(buf + ptr, len - ptr, "THREAD INFO:\n");
    ptr += snprintf(buf + ptr, len - ptr, "\t%-12s%-12s%-12s%-12s\n", "name", "priority", "free stack", "time percent");
    AcThread::getInfo(buf + ptr, len - ptr);
    return AC_OK;
}


