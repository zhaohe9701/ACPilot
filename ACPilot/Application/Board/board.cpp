//
// Created by zhaohe on 2023/8/16.
//
#include "board.h"
#include "Uart/uart_driver.h"
#include "Sensor/Imu/icm42688.h"
#include "Gpio/gpio_driver.h"
#include "Usb/usb_driver.h"
#include "DataModule/data_module.h"
//UartHandle uart1_handle;
//
//void handleInit()
//{
//    uart1_handle.index = UART_NUM_1;
//    uart1_handle.config.baud_rate = 115200;
//    uart1_handle.config.data_bits = UART_DATA_8_BITS;
//    uart1_handle.config.parity = UART_PARITY_DISABLE;
//    uart1_handle.config.stop_bits = UART_STOP_BITS_1;
//    uart1_handle.config.flow_ctrl = UART_HW_FLOWCTRL_DISABLE;
//    uart1_handle.config.source_clk = UART_SCLK_DEFAULT;
//    uart1_handle.tx = GPIO_NUM_4;
//    uart1_handle.rx = GPIO_NUM_5;
//}

Gpio *Board::imu_cs_pin = nullptr;
Gpio *Board::baro_cs_pin = nullptr;
Gpio *Board::flash_cs_pin = nullptr;
Gpio *Board::led_pin = nullptr;
Gpio *Board::imu_interrupt_pin = nullptr;
SpiBus *Board::spi1_bus = nullptr;
Uart *Board::uart1 = nullptr;
Usb *Board::usb = nullptr;
ExtInterrupt *Board::imu_interrupt = nullptr;

void test();

void boardInit()
{
//    /* gpio */
//    Gpio::enable();
//    Board::imu_cs_pin = new Gpio(IMU1_CS_GPIO_Port, IMU1_CS_Pin, GPIO_SET);
//    Board::imu_cs_pin->init(GPIO_PUSH_PULL, GPIO_NO_PULL, GPIO_HIGH_SPEED);
//
//    Board::baro_cs_pin = new Gpio(BARO_CS_GPIO_Port, BARO_CS_Pin, GPIO_SET);
//    Board::baro_cs_pin->init(GPIO_PUSH_PULL, GPIO_NO_PULL, GPIO_HIGH_SPEED);
//
//    Board::flash_cs_pin = new Gpio(FLASH_CS_GPIO_Port, FLASH_CS_Pin, GPIO_SET);
//    Board::flash_cs_pin->init(GPIO_PUSH_PULL, GPIO_NO_PULL, GPIO_HIGH_SPEED);
//
//    Board::led_pin = new Gpio(LED_GPIO_Port, LED_Pin, GPIO_RESET);
//    Board::led_pin->init(GPIO_OPEN_DRAIN, GPIO_NO_PULL, GPIO_LOW_SPEED);
//
//    Board::imu_interrupt_pin = new Gpio(SPI1_EXIT_GPIO_Port, SPI1_EXIT_Pin, GPIO_NONE);
//    Board::imu_interrupt_pin->init(GPIO_IT_RISING, GPIO_NO_PULL, GPIO_NONE_SPEED);
//    MX_DMA_Init();
//    MX_TIM8_Init();
//
//    Board::spi1_bus = new SpiBus();
//    Board::spi1_bus->init(SPI1, SPI_PRESCALER_16, SPI_CPOL_HIGH, SPI_CPHA_2);
//
//    Usb::test();
    Board::usb = new Usb(0x01);
    Board::usb->init();
    Board::usb->send((uint8_t*)"helloUSB\n", 9, 1000);
    test();
//    Board::uart1 = new Uart(&huart1, 0x02);
//    Board::uart1->init(USART1, 420000, UART_WORDLEN_8, UART_STOPBIT_10, UART_PARITY_N, UART_HW_CTRL_DISABLE);
//
//    Board::imu_interrupt = new ExtInterrupt(Board::imu_interrupt_pin, 5);
//    Board::imu_interrupt->init();
}

Led *Board::led = nullptr;
Imu *Board::imu1 = nullptr;
Dps310 *Board::baro = nullptr;

void deviceInit()
{
//    Board::led = new Led(Board::led_pin, LowActive);
//
//    Spi *imu_spi = new Spi(Board::spi1_bus, Board::imu_cs_pin);
//    Board::imu1 = new Icm42688(imu_spi);
//
//    Spi *baro_spi = new Spi(Board::spi1_bus, Board::baro_cs_pin);
//    Board::baro = new Dps310(baro_spi);
//
//    Spi *flash_spi = new Spi(Board::spi1_bus, Board::flash_cs_pin);
}

#include "DataModule/data_module.h"
#include "DataModule/data_node.h"
#include "DataModule/json.h"
void test()
{
    printf("test\n");
    char buf[200] = R"({"aircraft":{"weight":"uint32","size":[{"l":"int8","w":"int8"}],"max_speed":"uint32"},"remote":"uint8"})";
    DataModule::init(2048);
    DataNode *root = Json::createDm(buf);
    char json[200] = {0};
    Json::printDm(root, json, 200);
    printf("RESULT:%s\n", json);
    memset(json, 0, 200);
    memset(buf, 0, 200);
    strcpy(buf, R"({"aircraft":{"weight":"100","max_speed":"100"},"remote":"1"})");
    Json::setToDm(root, buf);
    Json::fromDm(root, json, 200);
    printf("RESULT:%s\n", json);
}


