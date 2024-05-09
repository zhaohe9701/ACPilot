//
// Created by zhaohe on 2023/8/17.
//
#include "Interrupt/interrupt_driver.h"
#include "driver/gpio.h"
#include "default_debug.h"

void extInterruptHandle(void *arg)
{
    ExtInterrupt *interrupt = (ExtInterrupt *) arg;
    interrupt->notify();
}

ExtInterrupt::ExtInterrupt(ExtInterruptHandle *handle)
{
    _handle = handle;
}

AC_RET ExtInterrupt::init()
{
    if (ESP_OK != gpio_config(&_handle->config))
    {
        BASE_ERROR("gpio config error");
        return AC_ERROR;
    }
    gpio_install_isr_service(0);
    if (ESP_OK != gpio_isr_handler_add(_handle->pin, extInterruptHandle, this))
    {
        BASE_ERROR("isr add error");
        return AC_ERROR;
    }

    return AC_OK;
}

void ExtInterrupt::notify()
{
    sem.give();
}

AC_RET ExtInterrupt::waitNotify(uint32_t timeout)
{
    return sem.get(timeout);
}
