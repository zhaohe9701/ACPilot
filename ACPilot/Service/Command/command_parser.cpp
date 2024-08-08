#include <stdio.h>
#include <string.h>
#include "command_parser.h"
#include "Mailbox/mailbox.h"
#include "Type/type.h"

using namespace Service;

CommandParser::CommandParser()
{
    _manager = Utils::Mailbox<CommandMessage>::find("command");
}

bool CommandParser::match(ComMessage &message)
{
    if (message.buf[0] == COMMAND_HEAD)
    {
        return true;
    }
    return false;
}

AC_RET CommandParser::parse(ComMessage &message, bool &free_message)
{
    CommandMessage command;
    uint32_t c = 0;

    free_message = false;
    for (uint32_t i = 1; i < message.len; ++i)
    {
        if (message.buf[i] == ' ')
        {
            message.buf[i] = '\0';
            continue;
        }
        if (message.buf[i - 1] == COMMAND_HEAD || message.buf[i - 1] == '\0')
        {
            command.argv[c] = (char *)(message.buf + i);
            c++;
        }
        if (message.buf[i] == '\r' || message.buf[i] == '\n')
        {
            message.buf[i] = '\0';
            break;
        }
    }
    command.argc = c;
    memcpy(&command.message, &message, sizeof(ComMessage));

    _manager->push(&command);
    return AC_OK;
}
