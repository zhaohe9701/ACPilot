//
// Created by zhaohe on 2024/4/26.
//

#ifndef REMOTE_MAPPER_H_
#define REMOTE_MAPPER_H_


#include "type.h"
#include "config.h"
#include "message.h"

class RemoteMapper
{
public:
    explicit RemoteMapper(uint8_t id);

    static AC_RET init(const char *url);

    uint16_t map(RemoteData &remote_data);

    bool isChanged() const;

private:
    uint16_t _old = 0;
    uint16_t _new = 0;
    uint8_t _id = 0;
    static uint8_t _map[MAX_CHANNEL_NUM];
    static uint8_t _sw[MAX_CHANNEL_NUM];
    static uint16_t _thr1[1];
    static uint16_t _thr2[2];

    static uint16_t _switchMap(uint16_t val, uint8_t switch_type);
};


#endif //REMOTE_MAPPER_H_
