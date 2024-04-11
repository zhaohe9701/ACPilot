//
// Created by zhaohe on 2024/4/8.
//

#ifndef MEMORY_POOL_FRAMEWORK_H_
#define MEMORY_POOL_FRAMEWORK_H_

#include "type.h"
#include "MemoryPool/memory_pool.h"

class MemoryPoolManager
{
public:
    static AC_RET init();

    static AC_RET info(char *buf, uint32_t len);
};


#endif // MEMORY_POOL_FRAMEWORK_H_
