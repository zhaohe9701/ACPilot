//
// Created by zhaohe on 2023/8/27.
//

#ifndef BOARD_H_
#define BOARD_H_
#include "Gpio/gpio_driver.h"
#include "Spi/spi_driver.h"
#include "Uart/uart_driver.h"
#include "Usb/usb_driver.h"
#include "Light/light.h"
#include "Sensor/Imu/imu.h"
#include "Interrupt/interrupt_driver.h"
#include "Sensor/Baro/dps310.h"

class Board
{
public:
    static Gpio *imu_cs_pin;
    static Gpio *baro_cs_pin;
    static Gpio *flash_cs_pin;
    static Gpio *led_pin;
    static Gpio *imu_interrupt_pin;

    static SpiBus *spi1_bus;
    static Uart *uart1;
    static Usb *usb;
    static ExtInterrupt *imu_interrupt;

    static Led *led;
    static Imu *imu1;
    static Dps310 *baro;
};

extern "C" void boardInit();
extern "C" void deviceInit();

#endif //BOARD_H_
