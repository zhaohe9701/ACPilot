//
// Created by zhaohe on 2023/11/19.
//

#include <stdarg.h>
#include <stdio.h>
#include "ac_debug.h"
#include "message.h"
#include "config.h"
// #include "usbd_cdc_if.h"

#define MAX_DEBUG_BUF_LENGTH MAX_MESSAGE_BUF_LEN
uint8_t Debug::_port = DEBUG_PORT_ID;
Mailbox<ComMessage> *Debug::_mail;
MemoryPool *Debug::_buffer_pool;

AC_RET Debug::init()
{
    _mail = Mailbox<ComMessage>::find("send");
    _buffer_pool = MemoryPool::find("message");
    return AC_OK;
}

void Debug::print(const char *format, ...)
{
    ComMessage message;
    va_list args;
    uint8_t *buffer = nullptr;

    buffer = _buffer_pool->alloc();
    va_start(args, format);
    message.len = vsnprintf((char*)buffer, MAX_DEBUG_BUF_LENGTH, (char *)format, args);
    va_end(args);
    message.buf = buffer;
    message.port = _port;
    _mail->push(&message);
}

// static char debug_buf[MAX_DEBUG_BUF_LENGTH] = {0};
// void debugPrintf(const char *format, ...)
// {
//     va_list args;
//     uint32_t len = 0;
//     va_start(args, format);
//     len = vsnprintf((char*)debug_buf, MAX_DEBUG_BUF_LENGTH, (char *)format, args);
//     va_end(args);
//     while (USBD_OK != CDC_Transmit_FS((uint8_t*)debug_buf, len)){}
// }