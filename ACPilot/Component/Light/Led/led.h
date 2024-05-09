//
// Created by zhaohe on 2024/4/19.
//

#ifndef LED_H_
#define LED_H_


#include "Light/light.h"
#include "Thread/ac_thread.h"
#include "Gpio/gpio_driver.h"

class Led : public Light
{
public:
    Led(Gpio *pin, GpioState on, LightMode mode, uint8_t id);

    AC_RET setMode(LightMode mode) override;

    void setBright(uint8_t bright, uint16_t time);

private:
    volatile LightMode _mode = LIGHT_KEEP_OFF;
    AcThread *_thread = nullptr;
    Gpio *_pin = nullptr;
    GpioState _on = GPIO_RESET;
    GpioState _off = GPIO_SET;

    static void _loop(void *param);
};


#endif //LED_H_
