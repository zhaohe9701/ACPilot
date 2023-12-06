#ifndef COMMAND_SERVER_H_
#define COMMAND_SERVER_H_

#include "Mail/mailbox.h"
#include "Debug/print.h"
#include "type.h"
#include "Command/command_parser.h"
#include "command.h"

class CommandServer
{
public:
    static AC_RET init();
    static void add(Command *command);
    static AC_RET run();
private:
    static Mailbox<CommandMessage> *_mailbox;
    static List<Command*> _list;
    static BufPool *_buffer_pool;
};

#endif
