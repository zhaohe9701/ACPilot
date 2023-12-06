//
// Created by zhaohe on 2023/6/18.
//

#ifndef __MESSAGE_H__
#define __MESSAGE_H__

#include "type.h"

class ComMessage
{
public:
    uint8_t port = 0x00;
    uint8_t *buf = nullptr;
    uint32_t len = 0;
};

#endif
