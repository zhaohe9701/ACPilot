/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-09-25 22:53:22
 * @LastEditors: zhaohe
 * @LastEditTime: 2022-12-24 20:42:27
 * @FilePath: \ZH_FLIGHT\Sys\Driver\z_pwm.cpp
 * Copyright (C) 2022 zhaohe. All rights reserved.
 */
#include "Pwm/pwm_driver.h"


void PwmCallbackHandle(TIM_HandleTypeDef *htim);

extern "C"
{
void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim)
{
    PwmCallbackHandle(htim);
}
}

static uint16_t HalAchiveMarkToChannel(uint16_t mark)
{
    uint16_t channel = 0x00U;
    switch (mark)
    {
        case 0x01U:
            channel = 0x00U;
            break;
        case 0x02U:
            channel = 0x04U;
            break;
        case 0x04U:
            channel = 0x08U;
            break;
        case 0x08U:
            channel = 0x0CU;
            break;
        default:
            break;
    }
    return channel;
}

void PwmCallbackHandle(TIM_HandleTypeDef *htim)
{
    HAL_TIM_PWM_Stop_DMA(htim, HalAchiveMarkToChannel(htim->Channel));
}


uint16_t Pwm::_pwmChannelToMark(uint16_t channel)
{
    uint16_t mark = 0x00U;
    switch (channel)
    {
        case 0x00U:
            mark = 0x01U;
            break;
        case 0x04U:
            mark = 0x02U;
            break;
        case 0x08U:
            mark = 0x04U;
            break;
        case 0x0CU:
            mark = 0x08U;
            break;
        default:
            break;
    }
    return mark;
}

Pwm::Pwm(TIM_HandleTypeDef *htim, uint16_t channel)
{
    _htim = htim;
    _channel = channel;
}


void Pwm::output(uint16_t *data, uint16_t length)
{
    HAL_TIM_PWM_Start_DMA(_htim, _channel, (uint32_t *) data, length);
}

void Pwm::stop()
{
    HAL_TIM_PWM_Stop_DMA(_htim, _channel);
}

bool Pwm::_isMe(TIM_HandleTypeDef *htim)
{
    if (htim == _htim && htim->Channel == _pwmChannelToMark(_channel))
    {
        return true;
    } else
    {
        return false;
    }
}
