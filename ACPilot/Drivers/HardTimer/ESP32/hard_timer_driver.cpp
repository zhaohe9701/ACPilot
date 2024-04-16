//
// Created by zhaohe on 2023/12/3.
//
#include "HardTimer/hard_timer_driver.h"

HardTimerHandle::HardTimerHandle()
{
    memset(&config, 0, sizeof(esp_timer_create_args_t));
}

HardwareTimer::HardwareTimer(const char *name, TaskFunction cb, void *param)
{
    if (nullptr == name)
    {
        return;
    }

    strncpy(_name, name, sizeof(_name));

    _handle.config.name = _name;
    _handle.config.callback = cb;
    _handle.config.arg = param;
}

AC_RET HardwareTimer::init()
{
    if (ESP_OK != esp_timer_create(&_handle.config, &_handle.handle))
    {
        return AC_ERROR;
    }
    return AC_OK;
}

AC_RET HardwareTimer::start(uint32_t timeout_us, TimerMode mode)
{
    if (TIMER_MODE_SINGLE == mode)
    {
        if (ESP_OK != esp_timer_start_once(_handle.handle, timeout_us))
        {
            return AC_ERROR;
        }
    } else
    {
        if (ESP_OK != esp_timer_start_periodic(_handle.handle, timeout_us))
        {
            return AC_ERROR;
        }
    }
    return AC_OK;
}

AC_RET HardwareTimer::stop()
{
    if (ESP_OK != esp_timer_stop(_handle.handle))
    {
        return AC_ERROR;
    }
    return AC_OK;
}

AC_RET HardwareTimer::del()
{
    if (ESP_OK != esp_timer_delete(_handle.handle))
    {
        return AC_ERROR;
    }
    return AC_OK;
}

bool HardwareTimer::isActive()
{
    return esp_timer_is_active(_handle.handle);
}
