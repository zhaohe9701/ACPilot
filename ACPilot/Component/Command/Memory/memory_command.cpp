//
// Created by zhaohe on 2024/4/9.
//

#include "memory_command.h"
#include "Debug/default_debug.h"
#include "MemoryPool/memory_pool_manager.h"
#include "Memory/memory.h"

#define MAX_MEMORY_REPLY_BUF_LEN 1024

using namespace Component;

MemoryCommand::MemoryCommand()
{
    strncpy(_cmd, "memory", CMD_MAX_LEN);
    _pool = Utils::MemoryPool::getGeneral(MAX_MEMORY_REPLY_BUF_LEN);
    if (nullptr == _pool)
    {
        BASE_ERROR("alloc error");
    }
}

int MemoryCommand::commandMain(int argc, char **argv)
{
    if (AC_OK != _alloc_result_buf())
    {
        BASE_ERROR("alloc error");
        return -1;
    }

    if (nullptr == argv || argc < 2)
    {
        snprintf(_res_buf, MAX_MEMORY_REPLY_BUF_LEN, "illegal param\n");
        return -1;
    }

    if (0 == strncmp(argv[1], "p_info", CMD_MAX_LEN))
    {
        if (AC_OK != MemoryPoolManager::info(_res_buf, MAX_MEMORY_REPLY_BUF_LEN))
        {
            snprintf(_res_buf, MAX_MEMORY_REPLY_BUF_LEN, "info failed\n");
            return -1;
        }
    } else if (0 == strncmp(argv[1], "m_info", CMD_MAX_LEN))
    {
        if (AC_OK != Osal::Memory::info(_res_buf, MAX_MEMORY_REPLY_BUF_LEN))
        {
            snprintf(_res_buf, MAX_MEMORY_REPLY_BUF_LEN, "info failed\n");
            return -1;
        }
    }
    else
    {
        snprintf(_res_buf, MAX_MEMORY_REPLY_BUF_LEN, "unknown cmd:%s\n", argv[1]);
    }
    return 0;
}


