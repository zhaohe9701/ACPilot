//
// Created by zhaohe on 2023/11/15.
//

#include "command_config.h"
#include "Command/command_server.h"
#include <string.h>

CommandConfig::CommandConfig()
{
    strncpy(_cmd, "cfg", CMD_MAX_LEN);
    CommandServer::add(this);
}

int CommandConfig::commandMain(int argc, char *argv[])
{
    if (argc == 4 && 0 == strncmp(argv[1], "set", CMD_MAX_LEN - 1))
    {

    } else if (argc == 3 && 0 == strncmp(argv[1], "get", CMD_MAX_LEN - 1))
    {

    } else
    {
        return -1;
    }
    return 0;
}