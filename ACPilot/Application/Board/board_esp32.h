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
#include "Sensor/Baro/dps310.h"
#include "Wlan/udp.h"
#include "Iic/iic_driver.h"

class Board
        {
                public:
                static Gpio *imu_cs_pin;
                static Gpio *baro_cs_pin;
                static Gpio *flash_cs_pin;
                static Gpio *led_pin;
                static Gpio *imu_interrupt_pin;

                static SpiBus *spi1_bus;
                static IicBus *iic1_bus;
                static Iic *iic1;
                static Uart *uart1;
                static Usb *usb;
                static Udp *udp;
                static ExtInterrupt *imu_interrupt;

                static Led *led;
                static Dps310 *baro;
        };

extern "C" void boardInit();
extern "C" void deviceInit();

#endif //BOARD_ESP32_H_
