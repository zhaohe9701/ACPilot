//
// Created by zhaohe on 2023/12/3.
//
#include "HardTimer/hard_timer_driver.h"

HardClockHandle::HardClockHandle()
{
    memset(&config, 0, sizeof(gptimer_config_t));
}

static bool clockCallback(gptimer_handle_t arg, const gptimer_alarm_event_data_t *ptr, void *pVoid)
{
    HardClock *clock = (HardClock *)pVoid;
    clock->tick();
    return false;
}

HardClock::HardClock(HardClockHandle *handle)
{
    _handle = handle;
}

AC_RET HardClock::init()
{
    gptimer_alarm_config_t alarm_config;
    gptimer_event_callbacks_t cbs;
    memset(&alarm_config, 0, sizeof(gptimer_alarm_config_t));
    memset(&cbs, 0, sizeof(gptimer_event_callbacks_t));
    alarm_config.reload_count = 0;
    alarm_config.alarm_count = _handle->tick;
    alarm_config.flags.auto_reload_on_alarm = true;
    cbs.on_alarm = clockCallback;
    /* 初始化定时器 */
    if (ESP_OK != gptimer_new_timer(&_handle->config, &_handle->handle))
    {
        return AC_ERROR;
    }
    /* 注册超时回调 */
    if (ESP_OK != gptimer_register_event_callbacks(_handle->handle, &cbs, this))
    {
        return AC_ERROR;
    }
    /* 使能定时器 */
    if (ESP_OK != gptimer_enable(_handle->handle))
    {
        return AC_ERROR;
    }
    /* 设置定时参数 */
    if (ESP_OK != gptimer_set_alarm_action(_handle->handle, &alarm_config))
    {
        return AC_ERROR;
    }
    return AC_OK;
}

AC_RET HardClock::start()
{
    if (ESP_OK != gptimer_start(_handle->handle))
    {
        return AC_ERROR;
    }
    return AC_OK;
}

AC_RET HardClock::stop()
{
    if (ESP_OK != gptimer_stop(_handle->handle))
    {
        return AC_ERROR;
    }
    return AC_OK;
}

void HardClock::add(HardTimer *timer)
{
    if (nullptr == _timer_list)
    {
        _timer_list = timer;
        timer->_next = nullptr;
        timer->_prev = nullptr;
        return;
    }
    timer->_next = _timer_list;
    timer->_prev = nullptr;
    _timer_list->_prev = timer;
    _timer_list = timer;
}

void HardClock::tick()
{
    if (nullptr == _timer_list)
    {
        return;
    }
    HardTimer *timer = _timer_list;
    while (nullptr != timer)
    {
        if (!timer->check())
        {
            HardTimer *tmp = timer;
            timer = timer->_next;
            remove(tmp);
            continue;
        }
        timer = timer->_next;
    }
}

AC_RET HardClock::remove(HardTimer *timer)
{
    if (nullptr == timer)
    {
        return AC_ERROR;
    }
    HardTimer *it = _timer_list;
    while (it != nullptr && it != timer)
    {
        it = it->_next;
    }
    if (nullptr == it)
    {
        return AC_ERROR;
    }
    it->_prev->_next = it->_next;
    it->_next->_prev = it->_prev;
    return AC_OK;
}

HardTimer::HardTimer(HardClock *clock)
{
    _clock = clock;
}

AC_RET HardTimer::start(uint32_t time, TimerMode mode)
{
    if (nullptr == _clock)
    {
        return AC_ERROR;
    }
    _alarm_count = time;
    _current_count = 0;
    _mode = mode;
    _is_running = true;
    _clock->add(this);
    return AC_OK;
}

AC_RET HardTimer::stop()
{
    if (nullptr == _clock)
    {
        return AC_WARN;
    }
    _is_running = false;
    return AC_OK;
}

AC_RET HardTimer::wait()
{
    return _semaphore.get();
}

bool HardTimer::check()
{
    if (!_is_running)
    {
        return false;
    }
    _current_count++;
    if (_current_count >= _alarm_count)
    {
        _current_count = 0;
        _semaphore.give();
        if (TIMER_MODE_SINGLE == _mode)
        {
            return false;
        }
    }
    return true;
}
