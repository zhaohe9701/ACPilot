//
// Created by zhaohe on 2024/4/19.
//

#ifndef LED_H_
#define LED_H_

#include "Light/light.h"
#include "Thread/thread.h"
#include "Gpio/gpio_driver.h"

namespace Component
{
    class Led : public Service::Light
    {
    public:
        Led(Driver::Gpio *pin, Driver::GpioState on, Service::LightMode mode, uint8_t id);

        AC_RET setMode(Service::LightMode mode) override;

        void setBright(uint8_t bright, uint16_t time);

    private:
        volatile Service::LightMode _mode = Service::LIGHT_KEEP_OFF;
        Osal::AcThread *_thread = nullptr;
        Driver::Gpio *_pin = nullptr;
        Driver::GpioState _on = Driver::GPIO_RESET;
        Driver::GpioState _off = Driver::GPIO_SET;

        static void _loop(void *param);
    };
}

#endif //LED_H_
