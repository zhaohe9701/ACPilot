//
// Created by zhaohe on 2023/11/19.
//

#ifndef DEBUG_H_
#define DEBUG_H_

#include "type.h"
#include "message.h"
#include "Mail/mailbox.h"
#include "MemoryPool/memory_pool.h"

class Debug
{
public:
    static AC_RET init();
    static void print(const char *format, ...);
private:
    static uint8_t _port;
    static Mailbox<ComMessage> *_mail;
    static BufPool *_buffer_pool;
};

#define AC_INFO(fmt, args...) Debug::print("\033[1;32m  ERROR(%s:%d):\t\033[0m" fmt, __func__, __LINE__, ## args)
#define AC_WARNING(fmt, args...) Debug::print("\033[1;33m  ERROR(%s:%d):\t\033[0m" fmt, __func__, __LINE__, ## args)
#define AC_ERROR(fmt, args...) Debug::print("\033[1;31m  ERROR(%s:%d):\t\033[0m" fmt, __func__, __LINE__, ## args)

void debugPrintf(const char *format, ...);

#endif //DEBUG_H_
