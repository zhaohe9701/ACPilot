//
// Created by zhaohe on 2023/11/14.
//
#include "mailbox.h"


MailboxCore::MailboxCore(const char *name)
{
    strncpy(_name, name, BOX_NAME_LEN);
}

bool MailboxCore::match(const char *name)
{
    if (0 == strncmp(_name, name, BOX_NAME_LEN))
    {
        return true;
    }
    return false;
}

List<MailboxCore*> MailboxManager::_list;

void MailboxManager::add(MailboxCore *core)
{
    _list.pushBack(core);
}