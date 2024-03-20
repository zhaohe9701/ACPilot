//
// Created by zhaohe on 2023/11/6.
//

#ifndef TRANSMIT_SERVER_H_
#define TRANSMIT_SERVER_H_

#include "config.h"
#include "type.h"
#include "Interface/com_interface.h"
#include "message_parser.h"
#include "Mail/mailbox.h"
#include "List/ac_list.h"

class MessageTransmitServer
{
public:
    static AC_RET init();
    static void add(ComInterface *interface);
    static AC_RET run();
private:
    static List<ComInterface*> _interface_list;
    static Mailbox<ComMessage>* _mailbox;
    static MemoryPool *_buffer_pool;
};

#endif //TRANSMIT_SERVER_H_
