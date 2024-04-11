//
// Created by zhaohe on 2023/8/10.
//
#include "ac_memory.h"
#include "os.h"  // assuming this includes the definitions for pvPortMalloc and vPortFree

/*
void* operator new(size_t size)
{
    return pvPortMalloc(size);
}

void operator delete(void* p) noexcept
{
    if (nullptr == p)
    {
        return;
    }
    vPortFree(p);
}

void* operator new[](size_t size)
{
    return pvPortMalloc(size);
}

void operator delete[](void* p) noexcept
{
    if (nullptr == p)
    {
        return;
    }
    vPortFree(p);
}
 */

uint32_t Memory::getFreeHeapSize()
{
    return xPortGetFreeHeapSize();
}

uint32_t Memory::getMinimumFreeHeapSize()
{
    return xPortGetMinimumEverFreeHeapSize();
}

AC_RET Memory::info(char *buf, uint32_t len)
{
    uint32_t ptr = 0;
    ptr += snprintf(buf + ptr, len - ptr, "MEMORY INFO:\n");

    ptr += snprintf(buf + ptr, len - ptr, "\t%-12s%-12s\n", "free", "minimum");

    snprintf(buf + ptr, len - ptr, "\t%-12lu%-12lu\n", getFreeHeapSize(), getMinimumFreeHeapSize());

    return AC_OK;
}
