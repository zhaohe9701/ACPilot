//
// Created by zhaohe on 2023/8/10.
//
#include "ac_memory.h"
#include "os.h"  // assuming this includes the definitions for pvPortMalloc and vPortFree

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