//
// Created by zhaohe on 2024/4/8.
//

#include "mailbox_manager.h"

AC_RET MailboxManager::info(char *buf, uint32_t len)
{
    uint32_t ptr = 0;
    List<MailboxCore *> *list = MailboxCore::getList();

    ptr += snprintf(buf + ptr, len - ptr, "MAILBOX INFO:\n");
    ptr += snprintf(buf + ptr, len - ptr, "\t%-12s%-12s\n", "name", "size");

    for (ListNode<MailboxCore *> *it = list->begin(); it != list->end(); it = it->getNext())
    {
        ptr += snprintf(buf + ptr, len - ptr, "\t%-12s%-12lu\n",
                        (**it)->getName(),
                        (**it)->getLen());
    }
    return AC_OK;
}
