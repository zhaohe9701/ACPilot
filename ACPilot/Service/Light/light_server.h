//
// Created by zhaohe on 2024/4/19.
//

#ifndef LIGHT_SERVER_H_
#define LIGHT_SERVER_H_


#include "type.h"
#include "light.h"
#include "ac_list.h"
#include "Mail/mailbox.h"
#include "Thread/ac_thread.h"

struct LightMessage
{
    uint8_t id = 0x0;
    LightMode mode = LIGHT_KEEP_OFF;
};

class LightServer
{
public:
    static AC_RET init();

    static AC_RET start();

    static void add(Light *light);
private:
    static List<Light*> _list;
    static Mailbox<LightMessage> *_mailbox;
    static AcThread *_light_task;

    static void _loop(void *param);
};


#endif //LIGHT_SERVER_H_
