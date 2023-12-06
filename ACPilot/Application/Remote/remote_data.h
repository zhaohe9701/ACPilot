/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2023-03-12 21:14:03
 * @LastEditors: zhaohe
 * @LastEditTime: 2023-03-12 21:22:05
 * @FilePath: \ZH_FLIGHT\Sys\Remote\remote_data.h
 * Copyright (C) 2023 zhaohe. All rights reserved.
 */
#ifndef REMOTE_DATA_H_
#define REMOTE_DATA_H_

#include "config.h"
#include "type.h"

#define REMOTE_MAX  10000
class RemoteData
{
public:
    uint16_t channel[MAX_CHANNEL_NUM] = {0};
    uint16_t channel_num  = 0;
};

#endif