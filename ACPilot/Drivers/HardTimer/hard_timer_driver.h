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
#include "Thread/ac_thread.h"

enum TimerMode
{
    TIMER_MODE_SINGLE,
    TIMER_MODE_PERIODIC
};

class HardwareTimer
{
public:
    HardwareTimer(const char *name, TaskFunction cb, void *param);

    AC_RET init();

    AC_RET start(uint32_t timeout_us, TimerMode mode);

    bool isActive();

    AC_RET stop();

    AC_RET del();

private:
    HardTimerHandle _handle{};
    char _name[PARAM_NAME_LEN] = {0};
};

#endif //HARD_TIMER_DRIVER_H_
