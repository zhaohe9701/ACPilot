//
// Created by zhaohe on 2023/11/15.
//

#ifndef COMMAND_H_
#define COMMAND_H_

#include "type.h"
#include "message.h"
#include "command_parser.h"
#include "MemoryPool/memory_pool.h"

#define CMD_MAX_LEN 16

class Command
{
public:
    Command();

    bool match(CommandMessage &message);

    virtual int commandMain(int argc, char *argv[]) = 0;

    AC_RET getReplay(ComMessage &replay);

    void setPort(uint8_t port);
protected:
    AC_RET _alloc_result_buf();
    uint8_t _port = 0x0;
    char _cmd[CMD_MAX_LEN] = {0};
    MemoryPool *_pool = nullptr;
    char *_res_buf = nullptr;
};


#endif //COMMAND_H_
