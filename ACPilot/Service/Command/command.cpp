//
// Created by zhaohe on 2023/11/15.
//

#include "command.h"
#include <string.h>
#include "Command/command_server.h"


Command::Command()
{
    CommandServer::add(this);
}

bool Command::match(CommandMessage &message)
{
    if (message.argc > 0 && 0 == strncmp(_cmd, message.argv[0], CMD_MAX_LEN))
    {
        return true;
    }
    return false;
}

AC_RET Command::_alloc_result_buf()
{
    if (nullptr == _pool)
    {
        return AC_ERROR;
    }

    _res_buf = (char *)_pool->alloc();

    if (nullptr == _res_buf)
    {
        return AC_ERROR;
    }
    return AC_OK;
}

AC_RET Command::getReplay(ComMessage &replay)
{
    replay.pool = _pool;
    replay.buf = (uint8_t *)_res_buf;
    replay.len = strlen(_res_buf);
    replay.port = _port;
    return AC_OK;
}

void Command::setPort(uint8_t port)
{
    _port = port;
}
