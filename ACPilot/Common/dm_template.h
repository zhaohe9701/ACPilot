//
// Created by zhaohe on 2024/4/26.
//

#ifndef DM_TEMPLATE_H_
#define DM_TEMPLATE_H_

#include "config.h"

;
#pragma pack(1)

struct RemoteConfig
{
    uint8_t map[MAX_CHANNEL_NUM] = {0};
    uint8_t sw[MAX_CHANNEL_NUM] = {0};
    uint16_t thr[3] = {0};
};

struct WlanConfig
{
    char mode[4] = {0};
    char ssid[16] = {0};
    char password[16] = {0};
};

struct PidConfig
{
    float kp = 0.0f;
    float ki = 0.0f;
    float kd = 0.0f;
    float int_limit = 0.0f;
    float out_limit = 0.0f;
    float d_filter_cut_freq = 0.0f;
};

#pragma pack()

#endif //DM_TEMPLATE_H_
