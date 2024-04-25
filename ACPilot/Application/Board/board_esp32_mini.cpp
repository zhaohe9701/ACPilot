//
// Created by zhaohe on 2023/8/16.
//
#include "board_esp32_mini.h"
#include "Uart/uart_driver.h"
#include "Sensor/Imu/icm42688.h"
#include "Gpio/gpio_driver.h"
#include "Usb/usb_driver.h"
#include "Wlan/wlan_driver.h"
#include "Sensor/Mag/qmc5883.h"
#include "Sensor/Baro/dps310.h"
#include "Device/Virtual/Accelerometer/accelerometer.h"
#include "Device/Virtual/Gyroscope/gyroscope.h"
#include "Device/Virtual/Altimeter/altimeter.h"

/* uart */
UartHandle esp32_mini_uart1_handle;
/* wlan */
WlanHandle wlan_handle;
/* udp */
UdpHandle udp_handle;
/* spi */
SpiBusHandle spi_bus_handle;
SpiHandle spi1_handle;
SpiHandle spi2_handle;
/* gpio */
GpioHandle gpio21_handle;   // led

void uartHandleInit()
{
    esp32_mini_uart1_handle.index = UART_NUM_1;
    esp32_mini_uart1_handle.config.baud_rate = 420000;
    esp32_mini_uart1_handle.config.data_bits = UART_DATA_8_BITS;
    esp32_mini_uart1_handle.config.parity = UART_PARITY_DISABLE;
    esp32_mini_uart1_handle.config.stop_bits = UART_STOP_BITS_1;
    esp32_mini_uart1_handle.config.flow_ctrl = UART_HW_FLOWCTRL_DISABLE;
    esp32_mini_uart1_handle.config.source_clk = UART_SCLK_DEFAULT;
    esp32_mini_uart1_handle.tx = GPIO_NUM_17;
    esp32_mini_uart1_handle.rx = GPIO_NUM_18;
}

void wlanHandleInit()
{
    wlan_handle.mode = WLAN_MODE_STA;
    strcpy((char *) wlan_handle.config.sta.ssid, "ZGL_2G");
    strcpy((char *) wlan_handle.config.sta.password, "77140019");
}

void udpHandleInit()
{
    udp_handle.port = 8888;
}

void spiHandleInit()
{
    spi_bus_handle.config.miso_io_num = GPIO_NUM_13;
    spi_bus_handle.config.mosi_io_num = GPIO_NUM_11;
    spi_bus_handle.config.sclk_io_num = GPIO_NUM_12;
    spi_bus_handle.config.quadwp_io_num = -1;
    spi_bus_handle.config.quadhd_io_num = -1;

    spi_bus_handle.host = SPI2_HOST;

    spi_bus_handle.dma = SPI_DMA_CH_AUTO;

    spi1_handle.config.address_bits = 8;
    spi1_handle.config.command_bits = 0;
    spi1_handle.config.clock_speed_hz = 10000000; // 10MHz
    spi1_handle.config.mode = 3;
    spi1_handle.config.spics_io_num = GPIO_NUM_10;
    spi1_handle.config.queue_size = 4;

    spi2_handle.config.address_bits = 8;
    spi2_handle.config.command_bits = 0;
    spi2_handle.config.clock_speed_hz = 10000000; // 10MHz
    spi2_handle.config.mode = 3;
    spi2_handle.config.spics_io_num = GPIO_NUM_8;
    spi2_handle.config.queue_size = 4;
}

void gpioInit()
{
    gpio21_handle.config.pin_bit_mask = 1U << GPIO_NUM_21;
    gpio21_handle.config.mode = GPIO_MODE_OUTPUT;
    gpio21_handle.pin = GPIO_NUM_21;
    gpio21_handle.state = GPIO_SET;
}

Gpio *Board::led_pin = nullptr;
Gpio *Board::imu_interrupt_pin = nullptr;
SpiBus *Board::spi_bus_1 = nullptr;
Spi *Board::spi1 = nullptr;
Spi *Board::spi2 = nullptr;
Uart *Board::uart1 = nullptr;
Usb *Board::usb = nullptr;
Udp *Board::udp = nullptr;
ExtInterrupt *Board::imu_interrupt = nullptr;

void test(void *param);

void boardInit()
{
    gpioInit();
    Board::led_pin = new Gpio(&gpio21_handle);
    Board::led_pin ->init();

    Board::usb = new Usb(0x01);
    Board::usb->init();

    uartHandleInit();
    Board::uart1 = new Uart(&esp32_mini_uart1_handle, 0x02);
    Board::uart1->init();

    spiHandleInit();
    Board::spi_bus_1 = new SpiBus(&spi_bus_handle);
    Board::spi_bus_1->init();
    Board::spi1 = new Spi(Board::spi_bus_1, &spi1_handle);
    Board::spi2 = new Spi(Board::spi_bus_1, &spi2_handle);
    Board::spi1->init();
    Board::spi2->init();

    udpHandleInit();
    Board::udp = new Udp(&udp_handle, 0x04);
    Board::udp->init();

    wlanHandleInit();
    WlanDriver::init(&wlan_handle);
}

void deviceInit()
{
    (new Icm42688(Board::spi1))->init();
    (new Dps310(Board::spi2))->init();

    (new Accelerometer("acc"))->bind(PhysicalDevice::find("ICM42688"));
    (new Gyroscope("gyro"))->bind(PhysicalDevice::find("ICM42688"));
    (new Altimeter("altimeter"))->bind(PhysicalDevice::find("DPS310"));

//    AcThread *test_thread = new AcThread("test", 4000, 20);
//    test_thread->run(test, nullptr);
}


void eventHandle(void *param);

void test(void *param)
{
    Notify::sub(ENTER_INIT_EVENT, eventHandle, nullptr);
    Notify::sub(ENTER_LOCK_EVENT, eventHandle, nullptr);
    Notify::sub(ENTER_UNLOCKING_EVENT, eventHandle, nullptr);
    Notify::sub(ENTER_READY_EVENT, eventHandle, nullptr);
    Notify::sub(ENTER_MANUAL_EVENT, eventHandle, nullptr);
    Notify::sub(ENTER_HEIGHT_EVENT, eventHandle, nullptr);
    Notify::sub(ENTER_CALIBRATE_EVENT, eventHandle, nullptr);

    Altimeter *alt = static_cast<Altimeter *>(VirtualDevice::find("altimeter", ALTIMETER_DEV));
    Accelerometer *acc = static_cast<Accelerometer *>(VirtualDevice::find("acc", ACCELEROMETER_DEV));
    while (1)
    {
        AccData data;
        acc->read(data);
        printf("acc: %f, %f, %f\n", data.x, data.y, data.z);
        AltitudeData alt_data;
        alt->read(alt_data);
        printf("alt: %f\n", alt_data.x);
        tickSleep(500);
    }
}

void eventHandle(void *param)
{
    NotifyToken *token = (NotifyToken *) param;
    switch (token->getEvent())
    {
        case ENTER_INIT_EVENT:
            printf("init finish event\n");
            break;
        case ENTER_LOCK_EVENT:
            printf("lock event\n");
            break;
        case ENTER_UNLOCKING_EVENT:
            printf("unlocking event\n");
            break;
        case ENTER_READY_EVENT:
            printf("ready event\n");
            break;
        case ENTER_MANUAL_EVENT:
            printf("manual event\n");
            break;
        case ENTER_HEIGHT_EVENT:
            printf("height event\n");
            break;
        case ENTER_CALIBRATE_EVENT:
            printf("calibrate event\n");
            break;
        default:
            break;
    }
}
