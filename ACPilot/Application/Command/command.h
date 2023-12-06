//
// Created by zhaohe on 2023/11/15.
//

#ifndef COMMAND_H_
#define COMMAND_H_
#include "type.h"
#include "command_parser.h"

#define CMD_MAX_LEN 16

class Command
{
public:
    bool match(CommandMessage &message);
    virtual int commandMain(int argc, char *argv[]) = 0;
protected:
    char _cmd[CMD_MAX_LEN] = {0};
};


#endif //COMMAND_H_
