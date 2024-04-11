//
// Created by zhaohe on 2023/11/6.
//

#ifndef TRANSMIT_SERVER_H_
#define TRANSMIT_SERVER_H_

#include "config.h"
#include "type.h"
#include "Com/com_interface.h"
#include "Mail/mailbox.h"
#include "ac_list.h"
#include "Thread/ac_thread.h"

class MessageTransmitServer
{
public:
    static AC_RET init();

    static AC_RET start();

private:
    static List<ComInterface *> *_interface_list;
    static Mailbox<ComMessage> *_mailbox;
    static AcThread *_transmit_task;

    static void _loop(void *param);
};

#endif //TRANSMIT_SERVER_H_
