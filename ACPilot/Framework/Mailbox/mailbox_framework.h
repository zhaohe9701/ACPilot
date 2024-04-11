//
// Created by zhaohe on 2024/4/8.
//

#ifndef MAILBOX_FRAMEWORK_H_
#define MAILBOX_FRAMEWORK_H_


#include "type.h"
#include "Mail/mailbox.h"

class MailboxFramework
{
public:
    static AC_RET info(char *buf, uint32_t len);
};


#endif //MAILBOX_FRAMEWORK_H_
