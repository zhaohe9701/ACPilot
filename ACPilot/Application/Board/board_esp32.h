//
// Created by zhaohe on 2023/8/27.
//

#ifndef BOARD_ESP32_H_
#define BOARD_ESP32_H_
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
    static Gpio *led_pin;
    static AdConverterUnit *adc_unit;
    static AdConverterChannel *adc_channel0;
    static ExtInterrupt *imu_interrupt;
    static SpiBus *spi_bus_1;
    static Spi *spi1;
    static Spi *spi2;
    static Uart *uart1;
    static Usb *usb;
    static Udp *udp;
    static Pwm *pwm0;
    static Pwm *pwm1;
    static Pwm *pwm2;
    static Pwm *pwm3;
};

extern "C" void boardInit();

extern "C" void deviceInit();

#endif //BOARD_ESP32_MINI_H_
