//
// Created by zhaohe on 2024/4/9.
//

#include "mailbox_command.h"

#include "default_debug.h"
#include "Mailbox/mailbox_framework.h"

#define MAX_MAILBOX_REPLY_BUF_LEN 1024

MailboxCommand::MailboxCommand()
{
    strncpy(_cmd, "mailbox", CMD_MAX_LEN);
    _pool = MemoryPool::getGeneral(MAX_MAILBOX_REPLY_BUF_LEN);
    if (nullptr == _pool)
    {
        BASE_ERROR("alloc error");
    }
}

int MailboxCommand::commandMain(int argc, char **argv)
{
    if (AC_OK != _alloc_result_buf())
    {
        BASE_ERROR("alloc error");
        return -1;
    }

    if (nullptr == argv || argc < 2)
    {
        snprintf(_res_buf, MAX_MAILBOX_REPLY_BUF_LEN, "illegal param\n");
        return -1;
    }

    if (0 == strncmp(argv[1], "info", CMD_MAX_LEN))
    {
        if (AC_OK != MailboxFramework::info(_res_buf, MAX_MAILBOX_REPLY_BUF_LEN))
        {
            snprintf(_res_buf, MAX_MAILBOX_REPLY_BUF_LEN, "info failed\n");
            return -1;
        }
    } else
    {
        snprintf(_res_buf, MAX_MAILBOX_REPLY_BUF_LEN, "unknown cmd:%s\n", argv[1]);
    }
    return 0;
}