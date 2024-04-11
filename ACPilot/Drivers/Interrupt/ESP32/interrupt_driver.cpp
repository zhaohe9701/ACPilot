//
// Created by zhaohe on 2023/8/17.
//
#include "Interrupt/interrupt_driver.h"
#include "driver/gpio.h"

void extInterruptHandle(void *arg)
{
    ExtInterrupt *interrupt = (ExtInterrupt*)arg;
    interrupt->notify();
}

ExtInterrupt::ExtInterrupt(ExtInterruptHandle *handle)
{
    _handle = handle;
}

AC_RET ExtInterrupt::init()
{
    if (ESP_OK != gpio_set_intr_type(_handle->gpio->getPin(), GPIO_INTR_NEGEDGE))
    {
        return AC_ERROR;
    }
    if (ESP_OK != gpio_isr_handler_add(_handle->gpio->getPin(), extInterruptHandle, this))
    {
        return AC_ERROR;
    }
    return AC_OK;
}

bool ExtInterrupt::matchPin(GpioPin pin)
{
    return _handle->gpio->getPin() == pin;
}

void ExtInterrupt::notify()
{
    sem.give();
}

AC_RET ExtInterrupt::waitNotify(uint32_t timeout)
{
    return sem.get(timeout);
}
