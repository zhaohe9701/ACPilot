#include "command_server.h"
#include "Mail/mailbox.h"
#include "type.h"
#include "default_debug.h"

#define COMMAND_TASK_PRIO  10
#define COMMAND_TASK_STACK_SIZE 3000

Mailbox<CommandMessage> *CommandServer::_cmd_mailbox = nullptr;
Mailbox<ComMessage> *CommandServer::_send_mailbox = nullptr;
List<Command *> CommandServer::_list;
AcThread *CommandServer::_command_task = nullptr;

AC_RET CommandServer::init()
{
    _cmd_mailbox = Mailbox<CommandMessage>::find("command");
    _send_mailbox = Mailbox<ComMessage>::find("send");
    if (nullptr == _cmd_mailbox || nullptr == _send_mailbox)
    {
        BASE_ERROR("object can't find");
        return AC_ERROR;
    }
    _command_task = new AcThread("command", COMMAND_TASK_STACK_SIZE, COMMAND_TASK_PRIO);
    return AC_OK;
}

AC_RET CommandServer::start()
{
    if (nullptr == _command_task)
    {
        BASE_ERROR("service not init");
        return AC_ERROR;
    }
    _command_task->run(_loop, nullptr);
    return AC_OK;
}

void CommandServer::add(Command *command)
{
    _list.pushBack(command);
}

void CommandServer::_loop(void *param)
{
    if (nullptr == _cmd_mailbox || nullptr == _send_mailbox)
    {
        BASE_ERROR("NULL ptr");
        AcThread::killSelf();
    }
    for (;;)
    {
        CommandMessage cmd;
        ComMessage replay_msg;

        _cmd_mailbox->pop(&cmd);

        for (ListNode<Command *> *it = _list.begin(); it != _list.end(); it = it->getNext())
        {
            if ((*(*it))->match(cmd))
            {
                (*(*it))->commandMain(cmd.argc, cmd.argv);
                (*(*it))->getReplay(replay_msg);
                replay_msg.port = cmd.message.port;
                _send_mailbox->push(&replay_msg);
            }
        }
        if (nullptr != cmd.message.pool)
        {
            static_cast<MemoryPool *>(cmd.message.pool)->free(cmd.message.buf);
        } else
        {
            BASE_ERROR("pool is null");
        }
    }
}


