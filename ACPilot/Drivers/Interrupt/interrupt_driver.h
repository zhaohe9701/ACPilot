//
// Created by zhaohe on 2023/10/17.
//

#ifndef INTERRUPT_DRIVER_H_
#define INTERRUPT_DRIVER_H_

#ifdef C_STM32
#include "STM32/interrupt_driver_param.h"
#endif

#ifdef C_ESP32

#include "ESP32/interrupt_driver_param.h"

#endif

#include "List/ac_list.h"
#include "Semaphore/semaphore.h"
#include "Gpio/gpio_driver.h"

namespace Driver
{
    class ExtInterrupt
    {
    public:
        explicit ExtInterrupt(ExtInterruptHandle *handle);

        AC_RET init();

        void notify();

        AC_RET waitNotify(uint32_t timeout = AC_FOREVER);

    private:
        ExtInterruptHandle *_handle = nullptr;
        Osal::BinSemaphore sem;

    };
}
#endif //INTERRUPT_DRIVER_H_
