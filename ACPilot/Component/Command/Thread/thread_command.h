//
// Created by zhaohe on 2024/4/9.
//

#ifndef THREAD_COMMAND_H_
#define THREAD_COMMAND_H_

#include "Command/command.h"

namespace Component
{
    class ThreadCommand : public Service::Command
    {
    public:
        ThreadCommand();

        int commandMain(int argc, char *argv[]) override;
    };
}
#endif //THREAD_COMMAND_H_
