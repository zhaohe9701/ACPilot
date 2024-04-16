//
// Created by zhaohe on 2023/8/17.
//
#include "main.h"
#include "Interrupt/interrupt_driver.h"

extern "C"
{
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    ExtInterrupt *interrupt = ExtInterruptManager::find(GPIO_Pin);
    if (nullptr == interrupt)
    {
        return;
    }
    interrupt->notify();
}
}


ExtInterrupt::ExtInterrupt(Gpio *gpio, uint32_t priority)
{
    _gpio = gpio;
    _priority = priority;
    ExtInterruptManager::add(this);
}

AC_RET ExtInterrupt::init()
{
    switch (_gpio->getPin())
    {
        case GPIO_PIN_0:
            HAL_NVIC_SetPriority(EXTI0_IRQn, _priority, 0);
            HAL_NVIC_EnableIRQ(EXTI0_IRQn);
            break;
        case GPIO_PIN_1:
            HAL_NVIC_SetPriority(EXTI1_IRQn, _priority, 0);
            HAL_NVIC_EnableIRQ(EXTI1_IRQn);
            break;
        case GPIO_PIN_2:
            HAL_NVIC_SetPriority(EXTI2_IRQn, _priority, 0);
            HAL_NVIC_EnableIRQ(EXTI2_IRQn);
            break;
        case GPIO_PIN_3:
            HAL_NVIC_SetPriority(EXTI3_IRQn, _priority, 0);
            HAL_NVIC_EnableIRQ(EXTI3_IRQn);
            break;
        case GPIO_PIN_4:
            HAL_NVIC_SetPriority(EXTI4_IRQn, _priority, 0);
            HAL_NVIC_EnableIRQ(EXTI4_IRQn);
            break;
        case GPIO_PIN_5:
        case GPIO_PIN_6:
        case GPIO_PIN_7:
        case GPIO_PIN_8:
        case GPIO_PIN_9:
            HAL_NVIC_SetPriority(EXTI9_5_IRQn, _priority, 0);
            HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);
            break;
        case GPIO_PIN_10:
        case GPIO_PIN_11:
        case GPIO_PIN_12:
        case GPIO_PIN_13:
        case GPIO_PIN_14:
        case GPIO_PIN_15:
            HAL_NVIC_SetPriority(EXTI15_10_IRQn, _priority, 0);
            HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
            break;
        default:
            return AC_ERROR;
    }
    return AC_OK;
}

bool ExtInterrupt::matchPin(GpioPin pin)
{
    return _gpio->getPin() == pin;
}

void ExtInterrupt::notify()
{
    sem.give();
}

AC_RET ExtInterrupt::waitNotify(uint32_t timeout)
{
    return sem.get(timeout);
}

List<ExtInterrupt *> ExtInterruptManager::_list;

void ExtInterruptManager::add(ExtInterrupt *interrupt)
{
    _list.pushBack(interrupt);
}

ExtInterrupt *ExtInterruptManager::find(GpioPin pin)
{
    for (ListNode < ExtInterrupt * > *it = _list.begin(); it != _list.end(); it = it->getNext())
    {
        if ((**it)->matchPin(pin))
        {
            return (**it);
        }
    }
    return nullptr;
}
