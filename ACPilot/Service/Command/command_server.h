#ifndef COMMAND_SERVER_H_
#define COMMAND_SERVER_H_

#include "Mail/mailbox.h"
#include "type.h"
#include "Command/command_parser.h"
#include "command.h"
#include "Thread/ac_thread.h"

class CommandServer
{
public:
    static AC_RET init();
    static AC_RET start();
    static void add(Command *command);
    static void _loop(void *param);
private:
    static Mailbox<CommandMessage> *_cmd_mailbox;
    static Mailbox<ComMessage> *_send_mailbox;
    static List<Command*> _list;
    static AcThread *_command_task;
};

#endif
