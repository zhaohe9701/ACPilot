//
// Created by zhaohe on 2023/5/22.
//

#ifndef LIGHT_H_
#define LIGHT_H_

#include <stdint.h>
#include "Gpio/gpio_driver.h"

enum LightPattern
{

    SLOW_FLASHING = 0,
    FAST_FLASHING = 1,
    BREATHE = 2,
    PULSE_FLASHING = 3,
    DOUBLE_PULSE_FLASHING = 4,
    KEEP_ON = 5,
    KEEP_OFF = 6
};

enum LightActiveLevel
{
    HighActive,
    LowActive
};

class Led
{
public:
    explicit Led(Gpio* led, LightActiveLevel level = LowActive);
    void setPattern(LightPattern pattern);
    void run();
private:
    void _breathe();
    void _slowFlashing();
    void _fastFlashing();
    void _pulseFlashing();
    void _doublePulseFlashing();
    void _keepOn();
    void _keepOff();

    void _openLed();
    void _closeLed();
    LightPattern _pattern = FAST_FLASHING;
    Gpio *_led = nullptr;
    LightActiveLevel _active_level = LowActive;
};


#endif //LIGHT_H_
