/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-09-25 02:04:02
 * @LastEditors: zhaohe
 * @LastEditTime: 2022-12-25 01:00:07
 * @FilePath: \ZH_FLIGHT\Sys\Driver\z_pwm.h
 * Copyright (C) 2022 zhaohe. All rights reserved.
 */
#ifndef PWM_DRIVER_PARAM_H_
#define PWM_DRIVER_PARAM_H_

#include "driver/ledc.h"
#include <string.h>

struct PwmHandle
{
    PwmHandle();

    ledc_timer_config_t timer_config{};
    ledc_channel_config_t channel_config{};
};

#endif