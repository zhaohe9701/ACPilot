//
// Created by zhaohe on 2024/4/19.
//

#ifndef LIGHT_SERVER_H_
#define LIGHT_SERVER_H_


#include "Type/type.h"
#include "light.h"
#include "List/ac_list.h"
#include "Mailbox/mailbox.h"
#include "Thread/thread.h"

namespace Service
{
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
        static Common::List<Light *> _list;
        static Utils::Mailbox<LightMessage> *_mailbox;
        static Osal::AcThread *_light_task;

        static void _loop(void *param);
    };
}

#endif //LIGHT_SERVER_H_
