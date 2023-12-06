//
// Created by zhaohe on 2023/11/15.
//

#ifndef COMMAND_CONFIG_H_
#define COMMAND_CONFIG_H_

#include "Command/command.h"

class CommandConfig : public Command
{
    CommandConfig();
    int commandMain(int argc, char *argv[]) override;
};


#endif //COMMAND_CONFIG_H_
