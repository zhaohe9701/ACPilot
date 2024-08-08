//
// Created by zhaohe on 2023/10/17.
//

#ifndef PWM_DRIVER_H_
#define PWM_DRIVER_H_

#ifdef C_STM32
#include "STM32/pwm_driver_param.h"
#endif

#ifdef C_ESP32

#include "ESP32/pwm_driver_param.h"

#endif

#include "Type/type.h"

namespace Driver
{

    class Pwm
    {
    public:
        explicit Pwm(PwmHandle *handle);

        AC_RET init();

        static AC_RET timerInit(PwmTimerHandle *handle);

        AC_RET set(float duty);

    private:
        PwmHandle *_handle = nullptr;
        static uint32_t _resolution;
    };
}
#endif //PWM_DRIVER_H_
