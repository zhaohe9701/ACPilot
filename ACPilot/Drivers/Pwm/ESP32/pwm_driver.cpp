//
// Created by zhaohe on 2023/11/27.
//
#include "Pwm/pwm_driver.h"

Pwm::Pwm(PwmHandle *handle)
{
    _handle = handle;
}

AC_RET Pwm::init()
{
    if(ESP_OK != ledc_timer_config(&_handle->timer_config))
    {
        return AC_ERROR;
    }
    if(ESP_OK != ledc_channel_config(&_handle->channel_config))
    {
        return AC_ERROR;
    }
    _resolution = (uint32_t)1 < _resolution - (uint32_t)1;
    return AC_OK;
}

AC_RET Pwm::set(float duty)
{
    uint32_t num = (uint32_t)(duty * (float)_resolution / 100.0f);
    if(ESP_OK != ledc_set_duty(_handle->channel_config.speed_mode, _handle->channel_config.channel, num))
    {
        return AC_ERROR;
    }
    if(ESP_OK != ledc_update_duty(LEDC_LOW_SPEED_MODE, _handle->channel_config.channel))
    {
        return AC_ERROR;
    }
    return AC_OK;
}