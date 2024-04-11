//
// Created by zhaohe on 2024/4/9.
//

#ifndef MAILBOX_COMMAND_H_
#define MAILBOX_COMMAND_H_

#include "Command/command.h"

class MailboxCommand : public Command {
public:
    MailboxCommand();
    int commandMain(int argc, char *argv[]) override;
};

#endif //MAILBOX_COMMAND_H_
