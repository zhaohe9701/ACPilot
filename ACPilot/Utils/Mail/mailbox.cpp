//
// Created by zhaohe on 2023/11/14.
//
#include "mailbox.h"

List<MailboxCore *> MailboxCore::_list;

MailboxCore::MailboxCore(const char *name, uint32_t len)
{
    strncpy(_name, name, BOX_NAME_LEN);
    _len = len;
}

bool MailboxCore::match(const char *name)
{
    if (0 == strncmp(_name, name, BOX_NAME_LEN))
    {
        return true;
    }
    return false;
}

char *MailboxCore::getName()
{
    return _name;
}

uint32_t MailboxCore::getLen()
{
    return _len;
}

List<MailboxCore *> *MailboxCore::getList()
{
    return &_list;
}
