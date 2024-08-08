//
// Created by zhaohe on 2023/11/6.
//

#ifndef TRANSMIT_SERVER_H_
#define TRANSMIT_SERVER_H_

#include "config.h"
#include "Type/type.h"
#include "Com/com.h"
#include "Mailbox/mailbox.h"
#include "List/ac_list.h"
#include "Thread/thread.h"

namespace Service
{
    class MessageTransmitServer
    {
    public:
        static AC_RET init();

        static AC_RET start();

    private:
        static Common::List<Interface::Com *> *_interface_list;
        static Utils::Mailbox<ComMessage> *_mailbox;
        static Osal::AcThread *_transmit_task;

        static void _loop(void *param);
    };
}
#endif //TRANSMIT_SERVER_H_
