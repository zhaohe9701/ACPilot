#ifndef COMMAND_SERVER_H_
#define COMMAND_SERVER_H_

#include "Mailbox/mailbox.h"
#include "Type/type.h"
#include "Command/command_parser.h"
#include "command.h"
#include "Thread/thread.h"

namespace Service
{
    class CommandServer
    {
    public:
        static AC_RET init();

        static AC_RET start();

        static void add(Command *command);

        static void _loop(void *param);

    private:
        static Utils::Mailbox<CommandMessage> *_cmd_mailbox;
        static Utils::Mailbox<ComMessage> *_send_mailbox;
        static Common::List<Command *> _list;
        static Osal::AcThread *_command_task;
    };
}
#endif
