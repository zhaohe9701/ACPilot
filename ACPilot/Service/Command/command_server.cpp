#include "command_server.h"
#include "Mailbox/mailbox.h"
#include "Type/type.h"
#include "Debug/default_debug.h"

using namespace Service;

Utils::Mailbox<CommandMessage> *CommandServer::_cmd_mailbox = nullptr;
Utils::Mailbox<ComMessage> *CommandServer::_send_mailbox = nullptr;
Common::List<Command *> CommandServer::_list;
Osal::AcThread *CommandServer::_command_task = nullptr;

AC_RET CommandServer::init()
{
    _cmd_mailbox = Utils::Mailbox<CommandMessage>::find("command");
    _send_mailbox = Utils::Mailbox<ComMessage>::find("send");
    if (nullptr == _cmd_mailbox || nullptr == _send_mailbox)
    {
        BASE_ERROR("object can't find");
        return AC_ERROR;
    }
    _command_task = new Osal::AcThread("command", COMMAND_TASK_STACK_SIZE, COMMAND_TASK_PRIO, COMMAND_TASK_CORE);
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
        Osal::AcThread::killSelf();
    }
    for (;;)
    {
        CommandMessage cmd;
        ComMessage replay_msg;

        _cmd_mailbox->pop(&cmd);

        for (Common::ListNode<Command *> *it = _list.begin(); it != _list.end(); it = it->getNext())
        {
            if ((*(*it))->match(cmd))
            {
                (*(*it))->setPort(cmd.message.port);
                (*(*it))->commandMain(cmd.argc, cmd.argv);
                (*(*it))->getReplay(replay_msg);
                if (0 != replay_msg.len)
                {
                    _send_mailbox->push(&replay_msg);
                } else
                {
                    if (nullptr != replay_msg.pool)
                    {
                        static_cast<Utils::MemoryPool *>(replay_msg.pool)->free(replay_msg.buf);
                    }
                }
            }
        }
        if (nullptr != cmd.message.pool)
        {
            static_cast<Utils::MemoryPool *>(cmd.message.pool)->free(cmd.message.buf);
        } else
        {
            BASE_ERROR("pool is null");
        }
    }
}


