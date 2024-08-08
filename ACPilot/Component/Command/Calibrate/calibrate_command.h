//
// Created by zhaohe on 2024/4/21.
//

#ifndef CALIBRATE_COMMAND_H_
#define CALIBRATE_COMMAND_H_

#include "Command/command.h"

namespace Component
{
    enum CaliCmd
    {
        CALI_CMD_ADVANCED,
        CALI_CMD_SIMPLE,
        CALI_CMD_SAMPLING,
        CALI_CMD_SAVE,
        CALI_CMD_STOP,
    };

    struct CaliMessage
    {
        uint8_t port = 0x0;
        CaliCmd cmd = CALI_CMD_STOP;
    };

    class CalibrateCommand : public Service::Command
    {
    public:
        CalibrateCommand();

        int commandMain(int argc, char *argv[]) override;

    private:
        Utils::Mailbox<CaliMessage> *_cali_cmd_mailbox = nullptr;
    };
}

#endif //CALIBRATE_COMMAND_H_
