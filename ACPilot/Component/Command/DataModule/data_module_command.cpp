//
// Created by zhaohe on 2024/4/8.
//

#include "data_module_command.h"
#include "DataModule/data_module_framework.h"
#include "MemoryPool/memory_pool_framework.h"
#include "default_debug.h"

#define  MAX_DATA_MODULE_REPLY_BUF_LEN 2048

DataModuleCommand::DataModuleCommand()
{
    strncpy(_cmd, "dm", CMD_MAX_LEN);
    _pool = MemoryPool::getGeneral(MAX_DATA_MODULE_REPLY_BUF_LEN);
    if (nullptr == _pool)
    {
        BASE_ERROR("alloc error");
    }
}

int DataModuleCommand::commandMain(int argc, char **argv)
{
    if (AC_OK != _alloc_result_buf())
    {
        BASE_ERROR("alloc error");
        return -1;
    }

    if (nullptr == argv || argc < 2)
    {
        snprintf(_res_buf, MAX_DATA_MODULE_REPLY_BUF_LEN, "illegal param\n");
        return -1;
    }
    if (0 != strncmp(argv[0], "dm", CMD_MAX_LEN))
    {
        snprintf(_res_buf, MAX_DATA_MODULE_REPLY_BUF_LEN, "wrong cmd:%s\n", argv[0]);
        return -1;
    }

    if (0 == strncmp(argv[1], "info", CMD_MAX_LEN))
    {
        if (AC_OK != DataModuleFramework::info(_res_buf, MAX_DATA_MODULE_REPLY_BUF_LEN))
        {
            snprintf(_res_buf, MAX_DATA_MODULE_REPLY_BUF_LEN, "info failed\n");
            return -1;
        }
    } else if (0 == strncmp(argv[1], "create", CMD_MAX_LEN))
    {
        if (AC_OK != DataModuleFramework::create(argv[2]))
        {
            snprintf(_res_buf, MAX_DATA_MODULE_REPLY_BUF_LEN, "get failed\n");
            return -1;
        }
    } else if (0 == strncmp(argv[1], "reset", CMD_MAX_LEN))
    {
        if (AC_OK != DataModuleFramework::reset())
        {
            snprintf(_res_buf, MAX_DATA_MODULE_REPLY_BUF_LEN, "reset failed\n");
            return -1;
        } else
        {
            snprintf(_res_buf, MAX_DATA_MODULE_REPLY_BUF_LEN, "reset success\n");
        }
    } else if (0 == strncmp(argv[1], "dump_data", CMD_MAX_LEN))
    {
        if (AC_OK != DataModuleFramework::dumpData(_res_buf, MAX_DATA_MODULE_REPLY_BUF_LEN))
        {
            snprintf(_res_buf, MAX_DATA_MODULE_REPLY_BUF_LEN, "dump data failed\n");
            return -1;
        }
    } else if (0 == strncmp(argv[1], "dump_struct", CMD_MAX_LEN))
    {
        if (AC_OK != DataModuleFramework::dumpStruct(_res_buf, MAX_DATA_MODULE_REPLY_BUF_LEN))
        {
            snprintf(_res_buf, MAX_DATA_MODULE_REPLY_BUF_LEN, "dump struct failed\n");
            return -1;
        }
    } else if (0 == strncmp(argv[1], "save", CMD_MAX_LEN))
    {
        if (AC_OK != DataModuleFramework::save())
        {
            snprintf(_res_buf, MAX_DATA_MODULE_REPLY_BUF_LEN, "save failed\n");
            return -1;
        }
        snprintf(_res_buf, MAX_DATA_MODULE_REPLY_BUF_LEN, "save success\n");
    } else if (0 == strncmp(argv[1], "load", CMD_MAX_LEN))
    {
        if (AC_OK != DataModuleFramework::load())
        {
            snprintf(_res_buf, MAX_DATA_MODULE_REPLY_BUF_LEN, "load failed\n");
            return -1;
        }
        snprintf(_res_buf, MAX_DATA_MODULE_REPLY_BUF_LEN, "load success\n");
    } else if (0 == strncmp(argv[1], "get", CMD_MAX_LEN))
    {
        if (AC_OK != DataModuleFramework::get(argv[2], _res_buf, MAX_DATA_MODULE_REPLY_BUF_LEN))
        {
            snprintf(_res_buf, MAX_DATA_MODULE_REPLY_BUF_LEN, "get failed\n");
            return -1;
        }
    } else if (0 == strncmp(argv[1], "set", CMD_MAX_LEN))
    {
        if (AC_OK != DataModuleFramework::set(argv[2], argv[3]))
        {
            snprintf(_res_buf, MAX_DATA_MODULE_REPLY_BUF_LEN, "set failed\n");
            return -1;
        }
        snprintf(_res_buf, MAX_DATA_MODULE_REPLY_BUF_LEN, "set success\n");
    } else if (0 == strncmp(argv[1], "add", CMD_MAX_LEN))
    {
        if (AC_OK != DataModuleFramework::add(argv[2], argv[3]))
        {
            snprintf(_res_buf, MAX_DATA_MODULE_REPLY_BUF_LEN, "add failed\n");
            return -1;
        }
        snprintf(_res_buf, MAX_DATA_MODULE_REPLY_BUF_LEN, "add success\n");
    } else if (0 == strncmp(argv[1], "del", CMD_MAX_LEN))
    {
        if (AC_OK != DataModuleFramework::del(argv[2]))
        {
            snprintf(_res_buf, MAX_DATA_MODULE_REPLY_BUF_LEN, "del failed\n");
            return -1;
        }
        snprintf(_res_buf, MAX_DATA_MODULE_REPLY_BUF_LEN, "del success\n");
    } else
    {
        snprintf(_res_buf, MAX_DATA_MODULE_REPLY_BUF_LEN, "unknown cmd:%s\n", argv[1]);
    }
    return 0;
}