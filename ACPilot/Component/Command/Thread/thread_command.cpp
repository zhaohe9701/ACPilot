//
// Created by zhaohe on 2024/4/9.
//

#include "thread_command.h"
#include "default_debug.h"
#include "Thread/thread_framework.h"

#define MAX_THREAD_REPLY_BUF_LEN 2048

ThreadCommand::ThreadCommand()
{
    strncpy(_cmd, "task", CMD_MAX_LEN);
    _pool = MemoryPool::getGeneral(MAX_THREAD_REPLY_BUF_LEN);
    if (nullptr == _pool)
    {
        BASE_ERROR("alloc error");
    }
}

int ThreadCommand::commandMain(int argc, char **argv)
{
    if (AC_OK != _alloc_result_buf())
    {
        BASE_ERROR("alloc error");
        return -1;
    }

    if (nullptr == argv || argc < 2)
    {
        snprintf(_res_buf, MAX_THREAD_REPLY_BUF_LEN, "illegal param\n");
        return -1;
    }

    if (0 == strncmp(argv[1], "info", CMD_MAX_LEN))
    {
        if (AC_OK != ThreadFramework::info(_res_buf, MAX_THREAD_REPLY_BUF_LEN))
        {
            snprintf(_res_buf, MAX_THREAD_REPLY_BUF_LEN, "info failed\n");
            return -1;
        }
    } else
    {
        snprintf(_res_buf, MAX_THREAD_REPLY_BUF_LEN, "unknown cmd:%s\n", argv[1]);
    }
    return 0;
}