//
// Created by zhaohe on 2023/8/27.
//

#ifndef BOARD_ESP32_MINI_H_
#define BOARD_ESP32_MINI_H_
#include "Gpio/gpio_driver.h"
#include "Spi/spi_driver.h"
#include "Uart/uart_driver.h"
#include "Usb/usb_driver.h"
#include "Light/light.h"
#include "Interrupt/interrupt_driver.h"
#include "Wlan/udp.h"
#include "Iic/iic_driver.h"
#include "Pwm/pwm_driver.h"
#include "AdConverter/ad_converter_driver.h"

class Board
{
public:
    static Driver::Gpio *led_pin;
    static Driver::AdConverterUnit *adc_unit;
    static Driver::AdConverterChannel *adc_channel0;
    static Driver::ExtInterrupt *imu_interrupt;
    static Driver::SpiBus *spi_bus_1;
    static Driver::Spi *spi1;
    static Driver::Spi *spi2;
    static Driver::Uart *uart1;
    static Driver::Usb *usb;
    static Driver::Udp *udp;
    static Driver::Pwm *pwm0;
    static Driver::Pwm *pwm1;
    static Driver::Pwm *pwm2;
    static Driver::Pwm *pwm3;
};

extern "C" void boardInit();

extern "C" void deviceInit();

#endif //BOARD_ESP32_MINI_H_
