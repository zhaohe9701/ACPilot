//
// Created by zhaohe on 2023/12/3.
//

#ifndef HARD_TIMER_DRIVER_H_
#define HARD_TIMER_DRIVER_H_

#ifdef C_STM32
#include "STM32/hard_timer_driver_param.h"
#endif

#ifdef C_ESP32
#include "ESP32/hard_timer_driver_param.h"
#endif

#include "type.h"
#include "Semaphore/ac_semaphore.h"

enum TimerMode
{
    TIMER_MODE_SINGLE,
    TIMER_MODE_PERIODIC
};

class HardTimer;

class HardClock
{
public:
    explicit HardClock(HardClockHandle *handle);
    AC_RET init();
    AC_RET start();
    AC_RET stop();
    void add(HardTimer *timer);
    AC_RET remove(HardTimer *timer);
    void tick();
private:
    HardClockHandle *_handle = nullptr;
    HardTimer *_timer_list = nullptr;
};

class HardTimer
{
    friend class HardClock;
public:
    explicit HardTimer(HardClock *clock);
    AC_RET start(uint32_t time, TimerMode mode);
    AC_RET stop();
    AC_RET wait();
    bool check();
private:
    HardTimer *_next = nullptr;
    HardTimer *_prev = nullptr;
    HardClock *_clock = nullptr;
    TimerMode _mode = TIMER_MODE_SINGLE;
    uint64_t _alarm_count = 0;
    uint64_t _current_count = 0;
    AcBinSemaphore _semaphore;
    bool _is_running = false;
};

#endif //HARD_TIMER_DRIVER_H_
