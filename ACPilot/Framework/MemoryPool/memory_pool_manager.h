//
// Created by zhaohe on 2024/4/8.
//

#ifndef MEMORY_POOL_MANAGER_H_
#define MEMORY_POOL_MANAGER_H_

#include "Type/type.h"
#include "MemoryPool/memory_pool.h"

class MemoryPoolManager
{
public:
    static AC_RET init();

    static AC_RET info(char *buf, uint32_t len);
};


#endif // MEMORY_POOL_MANAGER_H_
