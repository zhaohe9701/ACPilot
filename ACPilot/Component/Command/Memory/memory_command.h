//
// Created by zhaohe on 2024/4/9.
//

#ifndef MEMORY_POOL_COMMAND_H_
#define MEMORY_POOL_COMMAND_H_

#include "Command/command.h"

namespace Component
{

    class MemoryCommand : public Service::Command
    {
    public:
        MemoryCommand();

        int commandMain(int argc, char *argv[]) override;
    };

}

#endif //MEMORY_POOL_COMMAND_H_
