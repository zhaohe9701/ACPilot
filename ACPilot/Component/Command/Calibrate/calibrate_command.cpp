//
// Created by zhaohe on 2024/4/21.
//

#include "calibrate_command.h"
#include "default_debug.h"
#include "Notify/notify.h"

#define MAX_CALI_REPLY_BUF_LEN 128

CalibrateCommand::CalibrateCommand()
{
    strncpy(_cmd, "cali", CMD_MAX_LEN);
    _pool = MemoryPool::getGeneral(MAX_CALI_REPLY_BUF_LEN);
    if (nullptr == _pool)
    {
        BASE_ERROR("alloc error");
    }
    _cali_cmd_mailbox = Mailbox<CaliMessage>::find("cali");
    if (nullptr == _cali_cmd_mailbox)
    {
        BASE_ERROR("cali mailbox not find");
    }
}

int CalibrateCommand::commandMain(int argc, char **argv)
{
    if (AC_OK != _alloc_result_buf())
    {
        BASE_ERROR("alloc error");
        return -1;
    }

    if (nullptr == argv || argc < 2)
    {
        snprintf(_res_buf, MAX_CALI_REPLY_BUF_LEN, "illegal param\n");
        return -1;
    }

    if (nullptr == _cali_cmd_mailbox)
    {
        snprintf(_res_buf, MAX_CALI_REPLY_BUF_LEN, "cali mailbox not find\n");
        return -1;
    }

    CaliMessage cali_message{_port, CALI_CMD_STOP};

    if (0 == strncmp(argv[1], "advanced", CMD_MAX_LEN))
    {
        Notify::notify(CALI_COMMAND_EVENT);
        cali_message.cmd = CALI_CMD_ADVANCED;
        _cali_cmd_mailbox->push(&cali_message);
    } else if (0 == strncmp(argv[1], "simple", CMD_MAX_LEN))
    {
        Notify::notify(CALI_COMMAND_EVENT);
        cali_message.cmd = CALI_CMD_SIMPLE;
        _cali_cmd_mailbox->push(&cali_message);
    } else if (0 == strncmp(argv[1], "stop", CMD_MAX_LEN))
    {
        cali_message.cmd = CALI_CMD_STOP;
        _cali_cmd_mailbox->push(&cali_message);
    } else if (0 == strncmp(argv[1], "sampling", CMD_MAX_LEN))
    {
        cali_message.cmd = CALI_CMD_SAMPLING;
        _cali_cmd_mailbox->push(&cali_message);
    } else if (0 == strncmp(argv[1], "save", CMD_MAX_LEN))
    {
        cali_message.cmd = CALI_CMD_SAVE;
        _cali_cmd_mailbox->push(&cali_message);
    } else
    {
        snprintf(_res_buf, MAX_CALI_REPLY_BUF_LEN, "unknown cmd:%s\n", argv[1]);
    }
    return 0;
}