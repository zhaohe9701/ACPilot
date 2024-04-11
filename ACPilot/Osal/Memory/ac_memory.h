//
// Created by zhaohe on 2023/8/10.
//

#ifndef MEMORY_H_
#define MEMORY_H_

#include <stddef.h>  // for size_t
#include <stdint.h>
#include "type.h"

//void* operator new(size_t size);
//void operator delete(void* p) noexcept;
//
//void* operator new[](size_t size);
//void operator delete[](void* p) noexcept;

class Memory
{
public:
    static uint32_t getFreeHeapSize();

    static uint32_t getMinimumFreeHeapSize();

    static AC_RET info(char *buf, uint32_t len);

};
#endif //MEMORY_H_
