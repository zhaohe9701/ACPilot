//
// Created by zhaohe on 2024/4/8.
//

#ifndef DATA_MODULE_COMMAND_H_
#define DATA_MODULE_COMMAND_H_

#include "Command/command.h"

class DataModuleCommand : public Command
{
public:
    DataModuleCommand();

    int commandMain(int argc, char *argv[]) override;
};

#endif //DATA_MODULE_COMMAND_H_
