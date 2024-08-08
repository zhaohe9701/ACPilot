//
// Created by zhaohe on 2023/12/3.
//

#ifndef HARD_TIMER_DRIVER_PARAM_H_
#define HARD_TIMER_DRIVER_PARAM_H_

#include "esp_timer.h"
#include "string.h"

namespace Driver
{
    struct HardTimerHandle
    {
        HardTimerHandle();

        esp_timer_create_args_t config{};
        esp_timer_handle_t handle = nullptr;
    };
}

#endif //HARD_TIMER_DRIVER_PARAM_H_
