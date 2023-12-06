#include "command_server.h"
#include "Mail/mailbox.h"
#include "Debug/print.h"
#include "type.h"

BufPool *CommandServer::_buffer_pool = nullptr;
Mailbox<CommandMessage> *CommandServer::_mailbox = nullptr;
List<Command*> CommandServer::_list;

AC_RET CommandServer::init()
{
    _buffer_pool = BufPoolManager::find("message");
    _mailbox = MailboxManager::find<CommandMessage>("command");
    if (nullptr == _buffer_pool || nullptr == _mailbox)
    {
        return AC_ERROR;
    }
    return AC_OK;
}

void CommandServer::add(Command *command)
{
    _list.pushBack(command);
}

AC_RET CommandServer::run()
{
    CommandMessage message;
    _mailbox->pop(&message);
    for (ListNode<Command*> *it = _list.begin(); it != _list.end(); it = it->getNext())
    {
        if ((*(*it))->match(message))
        {
            (*(*it))->commandMain(message.argc, message.argv);
            _buffer_pool->free(message.message->buf);
            return AC_OK;
        }
    }
    _buffer_pool->free(message.message->buf);
    return AC_ERROR;
}
