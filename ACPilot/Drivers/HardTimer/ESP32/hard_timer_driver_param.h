//
// Created by zhaohe on 2023/12/3.
//

#ifndef HARD_TIMER_DRIVER_PARAM_H_
#define HARD_TIMER_DRIVER_PARAM_H_

#include "driver/gptimer.h"
#include "string.h"

struct HardClockHandle
{
    HardClockHandle();
    gptimer_config_t config{};
    gptimer_handle_t handle = nullptr;
    uint64_t tick = 0;
};

HardClockHandle::HardClockHandle()
{
    memset(&config, 0, sizeof(gptimer_config_t));
}

#endif //HARD_TIMER_DRIVER_PARAM_H_
