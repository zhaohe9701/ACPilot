//
// Created by zhaohe on 2024/4/23.
//

#include "nvs_command.h"
#include "Debug/default_debug.h"
#include "Nvs/nvs_driver.h"

#define MAX_NVS_REPLY_BUF_LEN 128

using namespace Component;

NvsCommand::NvsCommand()
{
    strncpy(_cmd, "nvs", CMD_MAX_LEN);
    _pool = Utils::MemoryPool::getGeneral(MAX_NVS_REPLY_BUF_LEN);
    if (nullptr == _pool)
    {
        BASE_ERROR("alloc error");
    }
}

int NvsCommand::commandMain(int argc, char **argv)
{
    if (AC_OK != _alloc_result_buf())
    {
        BASE_ERROR("alloc error");
        return -1;
    }

    if (nullptr == argv || argc < 2)
    {
        snprintf(_res_buf, MAX_NVS_REPLY_BUF_LEN, "illegal param\n");
        return -1;
    }

    if (0 == strncmp(argv[1], "info", CMD_MAX_LEN))
    {
        if (AC_OK != Driver::Nvs::info(_res_buf, MAX_NVS_REPLY_BUF_LEN))
        {
            snprintf(_res_buf, MAX_NVS_REPLY_BUF_LEN, "info failed\n");
            return -1;
        }
    } else if (0 == strncmp(argv[1], "erase", CMD_MAX_LEN))
    {
        if (argc < 3)
        {
            snprintf(_res_buf, MAX_NVS_REPLY_BUF_LEN, "illegal param\n");
            return -1;
        }
        if (AC_OK != Driver::Nvs::eraseAll(argv[2]))
        {
            snprintf(_res_buf, MAX_NVS_REPLY_BUF_LEN, "erase failed\n");
            return -1;
        }
        snprintf(_res_buf, MAX_NVS_REPLY_BUF_LEN, "erase success\n");
    } else
    {
        snprintf(_res_buf, MAX_NVS_REPLY_BUF_LEN, "unknown cmd:%s\n", argv[1]);
    }
    return 0;
}
