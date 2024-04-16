//
// Created by zhaohe on 2024/4/2.
//

#include "thread_manager.h"

AC_RET ThreadManager::info(char *buf, uint32_t len)
{
    uint32_t ptr = 0;
    ptr += snprintf(buf + ptr, len - ptr, "THREAD INFO:\n");
    ptr += snprintf(buf + ptr, len - ptr, "\t%-12s%-6s%-8s%-6s%-6s\n", "name", "prio", "stack", "time", "core");
    AcThread::getInfo(buf + ptr, len - ptr);
    return AC_OK;
}


