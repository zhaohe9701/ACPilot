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

class Board
{
public:
    static Gpio *led_pin;
    static Gpio *imu_interrupt_pin;

    static SpiBus *spi_bus_1;
    static Spi *spi1;
    static Spi *spi2;
    static Uart *uart1;
    static Usb *usb;
    static Udp *udp;
    static ExtInterrupt *imu_interrupt;
};

extern "C" void boardInit();

extern "C" void deviceInit();

#endif //BOARD_ESP32_MINI_H_
