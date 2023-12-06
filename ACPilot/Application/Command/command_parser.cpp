#include <stdio.h>
#include <string.h>
#include "command_parser.h"
#include "Mail/mailbox.h"
#include "type.h"
#include "sys.h"

CommandParser::CommandParser()
{
    _manager = MailboxManager::find<CommandMessage>("command");
    MessageReceiveServer::add(this);
}

bool CommandParser::match(ComMessage &message)
{
    if (message.buf[0] == COMMAND_HEAD)
    {
        return true;
    }
    return false;
}

AC_RET CommandParser::parseMessage(ComMessage &message)
{
    CommandMessage command;
    uint32_t v = 0;
    for (uint32_t i = 1; i < message.len; ++i)
    {
        if (message.buf[i] == ' ')
        {
            continue;
        }
        if (message.buf[i - 1] == COMMAND_HEAD || message.buf[i - 1] == ' ')
        {
            command.argv[v] = (char *)(message.buf + i);
            v++;
        }
    }
    command.argc = v;

    _manager->push(&command);
    return AC_OK;
}
