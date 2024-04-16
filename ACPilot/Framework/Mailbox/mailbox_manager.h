//
// Created by zhaohe on 2024/4/8.
//

#ifndef MAILBOX_MANAGER_H_
#define MAILBOX_MANAGER_H_


#include "type.h"
#include "Mail/mailbox.h"

class MailboxManager
{
public:
    static AC_RET info(char *buf, uint32_t len);
};


#endif //MAILBOX_MANAGER_H_
