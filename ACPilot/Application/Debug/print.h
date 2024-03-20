/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2023-01-23 16:31:55
 * @LastEditors: zhaohe
 * @LastEditTime: 2023-03-26 21:18:47
 * @FilePath: \ZH_FLIGHT\Sys\Debug\print.h
 * Copyright (C) 2023 zhaohe. All rights reserved.
 */
#ifndef PRINT_H_
#define PRINT_H_

#include <stdint.h>
#include "Queue/ac_queue.h"
#include "type.h"
#include "Mail/mailbox.h"
#include "message.h"
#include "MemoryPool/memory_pool.h"


#define MAX_PRINT_LENGTH 1024

class Printer
{
public:
    explicit Printer(uint8_t port);
    void print(const char *format, ...);
    void error(const char *format, ...);
    void info(const char *format, ...);
    void transmit(uint8_t *buf, uint32_t len);
private:
    uint8_t _port = 0x00;
    Mailbox<ComMessage> *_mail = nullptr;
    MemoryPool *_buffer_pool = nullptr;
};

#endif