//
// Created by zhaohe on 2024/4/8.
//

#include "memory_pool_manager.h"
#include "config.h"

static uint16_t per_general_memory_pool_num[GENERAL_MEMORY_POOL_NUM] = PER_GENERAL_MEMORY_POOL_NUM;
static uint16_t per_general_memory_pool_size[GENERAL_MEMORY_POOL_NUM] = PER_GENERAL_MEMORY_POOL_SIZE;

AC_RET MemoryPoolManager::init()
{
    char name[BUF_POOL_NAME_LEN] = {0};
    Utils::MemoryPool **general_list = Utils::MemoryPool::getGeneralPoolList();
    for (uint16_t i = 0; i < GENERAL_MEMORY_POOL_NUM; ++i)
    {
        snprintf(name, BUF_POOL_NAME_LEN, "pool_%d", per_general_memory_pool_size[i]);
        general_list[i] = new Utils::MemoryPool(name,
                                         per_general_memory_pool_size[i],
                                         per_general_memory_pool_num[i],
                                         true);
    }
    return AC_OK;
}


AC_RET MemoryPoolManager::info(char *buf, uint32_t len)
{
    uint32_t ptr = 0;
    Common::List<Utils::MemoryPool *> *list = Utils::MemoryPool::getList();
    ptr += snprintf(buf + ptr, len - ptr, "MEMORY POOL INFO:\n");
    ptr += snprintf(buf + ptr, len - ptr, "\t%-12s%-12s%-12s%-12s\n", "name", "total", "used", "free");
    for (Common::ListNode<Utils::MemoryPool *> *it = list->begin(); it != list->end(); it = it->getNext())
    {
        ptr += snprintf(buf + ptr, len - ptr, "\t%-12s%-12lu%-12lu%-12lu\n",
                        (**it)->getName(),
                        (**it)->getNum(),
                        (**it)->getUsedNum(),
                        (**it)->getFreeNum());
    }
    return AC_OK;
}