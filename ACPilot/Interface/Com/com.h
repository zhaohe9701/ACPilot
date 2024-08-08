/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-10-22 00:50:12
 * @LastEditors: zhaohe
 * @LastEditTime: 2023-02-27 23:00:42
 * @FilePath: \ZH_FLIGHT\Sys\Communicate\communicate_interface.h
 * Copyright (C) 2022 zhaohe. All rights reserved.
 */
#ifndef COM_H_
#define COM_H_

#include <stdint.h>
#include "Type/type.h"
#include "config.h"
#include "List/ac_list.h"
#include "MemoryPool/memory_pool.h"
#include "Mailbox/mailbox.h"
#include "message.h"
#include "Semaphore/semaphore.h"

namespace Interface
{
    class Com
    {
    public:
        explicit Com(uint8_t port_num);

        Com() = default;

        virtual AC_RET init();

        virtual AC_RET send(uint8_t *buf, uint16_t length, uint32_t timeout) = 0;

        virtual AC_RET open();

        AC_RET receive(uint32_t len);

        virtual bool match(uint8_t port_num);

        virtual ~Com() = default;

        static void add(Com *com);

        static Common::List<Com *> *getList();

    protected:
        uint8_t _port = 0x0;
        uint8_t *_recv_buffer = nullptr;
        uint8_t *_transmit_buffer = nullptr;
        Utils::Mailbox<ComMessage> *_recv_mail_box = nullptr;
        Utils::MemoryPool *_recv_pool = nullptr;
        static Common::List<Com *> _list;

    };
}
#endif