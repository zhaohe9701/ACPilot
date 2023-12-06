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
#ifndef COM_INTERFACE_H_
#define COM_INTERFACE_H_

#include <stdint.h>
#include "type.h"
#include "config.h"
#include "List/ac_list.h"
#include "MemoryPool/memory_pool.h"
#include "Mail/mailbox.h"
#include "message.h"
#include "Semaphore/ac_semaphore.h"

class ComInterface
{
public:
    explicit ComInterface(uint8_t port_num);
    virtual AC_RET send(uint8_t *buf, uint16_t length, uint32_t timeout) = 0;
    virtual AC_RET open() = 0;
    AC_RET receive(uint32_t len);

    virtual bool match(uint8_t port_num);
    virtual ~ComInterface()= default;
protected:
    uint8_t _port = 0x0;
    uint8_t *_receive_buffer = nullptr;
    uint8_t *_transmit_buffer = nullptr;
    BufPool *_buffer_pool = nullptr;
    Mailbox<ComMessage> *_recv_mail_box = nullptr;
};

#endif