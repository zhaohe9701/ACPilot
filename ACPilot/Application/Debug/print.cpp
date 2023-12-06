/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2023-01-23 16:34:50
 * @LastEditors: zhaohe
 * @LastEditTime: 2023-03-26 23:17:21
 * @FilePath: \ZH_FLIGHT\Sys\Debug\print.cpp
 * Copyright (C) 2023 zhaohe. All rights reserved.
 */
#include "print.h"
#include "Queue/ac_queue.h"
#include "type.h"
#include "message.h"
#include "Command/command_parser.h"
#include "usbd_def.h"
#include "usbd_cdc_if.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#define ERROR   ">[ERROR]:"
#define INFO    ">[INFO]:"
#define ERROR_LEN   9
#define INFO_LEN    8

Printer::Printer(uint8_t port)
{
    _port = port;
    _mail = MailboxManager::find<ComMessage>("send");
    _buffer_pool = BufPoolManager::find("message");
}

void Printer::print(const char *format, ...)
{
    ComMessage message;
    va_list args;
    uint8_t *buffer = nullptr;

    buffer = _buffer_pool->alloc();
    va_start(args, format);
    message.len = vsnprintf((char*)buffer, MAX_PRINT_LENGTH, (char *)format, args);
    va_end(args);
    message.buf = buffer;
    message.port = _port;
    _mail->push(&message);
}

void Printer::error(const char *format, ...)
{
    ComMessage message;
    va_list args;
    uint8_t *buffer = nullptr;

    buffer = _buffer_pool->alloc();
    va_start(args, format);
    message.len = vsnprintf((char*)buffer + ERROR_LEN, MAX_PRINT_LENGTH, (char *)format, args);
    va_end(args);
    message.len += ERROR_LEN;
    memcpy(buffer, ERROR, ERROR_LEN);
    message.buf = buffer;
    message.port = _port;
    _mail->push(&message);
}

void Printer::info(const char *format, ...)
{
    ComMessage message;
    va_list args;
    uint8_t *buffer = nullptr;

    buffer = _buffer_pool->alloc();
    va_start(args, format);
    message.len = vsnprintf((char*)buffer + INFO_LEN, MAX_PRINT_LENGTH, (char *)format, args);
    va_end(args);
    message.len += INFO_LEN;
    memcpy(buffer, INFO, INFO_LEN);
    message.buf = buffer;
    message.port = _port;
    _mail->push(&message);
}

void Printer::transmit(uint8_t *buf, uint32_t len)
{
    ComMessage message;
    uint8_t *buffer = nullptr;

    buffer = _buffer_pool->alloc();
    memcpy(buffer, buf, len);
    message.len = len;
    message.buf = buffer;
    message.port = _port;
    _mail->push(&message);
}
