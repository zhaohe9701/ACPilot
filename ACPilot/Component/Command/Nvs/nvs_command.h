//
// Created by zhaohe on 2024/4/23.
//

#ifndef NVS_COMMAND_H_
#define NVS_COMMAND_H_


#include "Command/command.h"

class NvsCommand : public Command
{
public:
    NvsCommand();

    int commandMain(int argc, char *argv[]) override;
};


#endif //NVS_COMMAND_H
