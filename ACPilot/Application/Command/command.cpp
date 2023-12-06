//
// Created by zhaohe on 2023/11/15.
//

#include "command.h"
#include <string.h>

bool Command::match(CommandMessage &message)
{
    if (message.argc > 0 && 0 == strncmp(_cmd, message.argv[0], CMD_MAX_LEN))
    {
        return true;
    }
    return false;
}