//
// Created by zhaohe on 2023/8/16.
//
#include "board_esp32.h"
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
#include "DataModule/data_module.h"
#include "Light/light_server.h"
#include "Sensor/Battery/build_in_battery.h"
#include "Device/Virtual/Coulometer/coulometer.h"
#include "Device/Virtual/Voltmeter/voltmeter.h"

/* uart */
Driver::UartHandle esp32_mini_uart1_handle;
/* wlan */
Driver::WlanHandle wlan_handle;
/* udp */
Driver::UdpHandle udp_handle;
/* spi */
Driver::SpiBusHandle spi_bus_handle;
Driver::SpiHandle spi1_handle;
Driver::SpiHandle spi2_handle;
/* gpio */
Driver::GpioHandle gpio21_handle;   // led
/* pwm */
Driver::PwmTimerHandle pwm_timer_handle;
Driver::PwmHandle pwm0_handle;
Driver::PwmHandle pwm1_handle;
Driver::PwmHandle pwm2_handle;
Driver::PwmHandle pwm3_handle;
/* interrupt */
Driver::ExtInterruptHandle imu_interrupt_handle;
/* adc */
Driver::AdConverterUnitHandle adc_unit_handle;
Driver::AdConverterChannelHandle adc_channel0_handle;

void adcInit()
{
    adc_unit_handle.cfg.unit_id = ADC_UNIT_1;
    adc_channel0_handle.cfg.bitwidth = ADC_BITWIDTH_DEFAULT;
    adc_channel0_handle.cfg.atten = ADC_ATTEN_DB_12;
    adc_channel0_handle.channel = ADC_CHANNEL_2;

}

void interruptHandleInit()
{
    imu_interrupt_handle.config.intr_type = GPIO_INTR_POSEDGE;
    imu_interrupt_handle.config.pin_bit_mask = 1U << GPIO_NUM_8;
    imu_interrupt_handle.config.mode = GPIO_MODE_INPUT;
    imu_interrupt_handle.config.pull_down_en = GPIO_PULLDOWN_ENABLE;
    imu_interrupt_handle.pin = GPIO_NUM_8;
}

void pwmHandleInit()
{
    pwm0_handle.channel_config.speed_mode = LEDC_LOW_SPEED_MODE;
    pwm0_handle.channel_config.channel = LEDC_CHANNEL_0;
    pwm0_handle.channel_config.timer_sel = LEDC_TIMER_0;
    pwm0_handle.channel_config.gpio_num = GPIO_NUM_34;

    pwm1_handle.channel_config.speed_mode = LEDC_LOW_SPEED_MODE;
    pwm1_handle.channel_config.channel = LEDC_CHANNEL_1;
    pwm1_handle.channel_config.timer_sel = LEDC_TIMER_0;
    pwm1_handle.channel_config.gpio_num = GPIO_NUM_33;

    pwm2_handle.channel_config.speed_mode = LEDC_LOW_SPEED_MODE;
    pwm2_handle.channel_config.channel = LEDC_CHANNEL_2;
    pwm2_handle.channel_config.timer_sel = LEDC_TIMER_0;
    pwm2_handle.channel_config.gpio_num = GPIO_NUM_17;

    pwm3_handle.channel_config.speed_mode = LEDC_LOW_SPEED_MODE;
    pwm3_handle.channel_config.channel = LEDC_CHANNEL_3;
    pwm3_handle.channel_config.timer_sel = LEDC_TIMER_0;
    pwm3_handle.channel_config.gpio_num = GPIO_NUM_18;

    pwm_timer_handle.timer_config.speed_mode = LEDC_LOW_SPEED_MODE;
    pwm_timer_handle.timer_config.timer_num = LEDC_TIMER_0;
    pwm_timer_handle.timer_config.duty_resolution = LEDC_TIMER_8_BIT;
    pwm_timer_handle.timer_config.freq_hz = 15000;

}

void uartHandleInit()
{
    esp32_mini_uart1_handle.index = UART_NUM_1;
    esp32_mini_uart1_handle.config.baud_rate = 420000;
    esp32_mini_uart1_handle.config.data_bits = UART_DATA_8_BITS;
    esp32_mini_uart1_handle.config.parity = UART_PARITY_DISABLE;
    esp32_mini_uart1_handle.config.stop_bits = UART_STOP_BITS_1;
    esp32_mini_uart1_handle.config.flow_ctrl = UART_HW_FLOWCTRL_DISABLE;
    esp32_mini_uart1_handle.config.source_clk = UART_SCLK_DEFAULT;
    esp32_mini_uart1_handle.tx = GPIO_NUM_37;
    esp32_mini_uart1_handle.rx = GPIO_NUM_38;
}

void wlanHandleInit()
{
    WlanConfig config;
    RETURN_CHECK(Framework::DataModule::read("/wlan", &config, sizeof(WlanConfig)));
    if (strcmp(config.mode, "AP") == 0)
    {
        wlan_handle.mode = Driver::WLAN_MODE_AP;
        strcpy((char *) wlan_handle.config.ap.ssid, config.ssid);
        strcpy((char *) wlan_handle.config.ap.password, config.password);
    } else
    {
        wlan_handle.mode = Driver::WLAN_MODE_STA;
        strcpy((char *) wlan_handle.config.sta.ssid, config.ssid);
        strcpy((char *) wlan_handle.config.sta.password, config.password);
    }
    return;
    error:
    BASE_ERROR("wlan init error");
}

void udpHandleInit()
{
    udp_handle.port = 8888;
}

void spiHandleInit()
{
    spi_bus_handle.config.miso_io_num = GPIO_NUM_7;
    spi_bus_handle.config.mosi_io_num = GPIO_NUM_6;
    spi_bus_handle.config.sclk_io_num = GPIO_NUM_5;
    spi_bus_handle.config.quadwp_io_num = -1;
    spi_bus_handle.config.quadhd_io_num = -1;

    spi_bus_handle.host = SPI2_HOST;

    spi_bus_handle.dma = SPI_DMA_CH_AUTO;

    spi1_handle.config.address_bits = 8;
    spi1_handle.config.command_bits = 0;
    spi1_handle.config.clock_speed_hz = 10000000; // 10MHz
    spi1_handle.config.mode = 3;
    spi1_handle.config.spics_io_num = GPIO_NUM_4;
    spi1_handle.config.queue_size = 4;

    spi2_handle.config.address_bits = 8;
    spi2_handle.config.command_bits = 0;
    spi2_handle.config.clock_speed_hz = 10000000; // 10MHz
    spi2_handle.config.mode = 3;
    spi2_handle.config.spics_io_num = GPIO_NUM_9;
    spi2_handle.config.queue_size = 4;
}

void gpioInit()
{
    gpio21_handle.config.pin_bit_mask = (uint64_t)1 << GPIO_NUM_21;
    gpio21_handle.config.mode = GPIO_MODE_OUTPUT;
    gpio21_handle.pin = GPIO_NUM_21;
    gpio21_handle.state = Driver::GPIO_SET;
}

Driver::Gpio *Board::led_pin = nullptr;
Driver::AdConverterUnit *Board::adc_unit = nullptr;
Driver::AdConverterChannel *Board::adc_channel0 = nullptr;
Driver::ExtInterrupt *Board::imu_interrupt = nullptr;
Driver::SpiBus *Board::spi_bus_1 = nullptr;
Driver::Spi *Board::spi1 = nullptr;
Driver::Spi *Board::spi2 = nullptr;
Driver::Uart *Board::uart1 = nullptr;
Driver::Usb *Board::usb = nullptr;
Driver::Udp *Board::udp = nullptr;
Driver::Pwm *Board::pwm0 = nullptr;
Driver::Pwm *Board::pwm1 = nullptr;
Driver::Pwm *Board::pwm2 = nullptr;
Driver::Pwm *Board::pwm3 = nullptr;

void boardInit()
{
    gpioInit();
    Board::led_pin = new Driver::Gpio(&gpio21_handle);
    Board::led_pin ->init();

    Board::usb = new Driver::Usb(USB_PORT_ID);
    Board::usb->init();

    uartHandleInit();
    Board::uart1 = new Driver::Uart(&esp32_mini_uart1_handle, UART1_PORT_ID);
    Board::uart1->init();

    interruptHandleInit();
    Board::imu_interrupt = new Driver::ExtInterrupt(&imu_interrupt_handle);
    Board::imu_interrupt->init();

    spiHandleInit();
    Board::spi_bus_1 = new Driver::SpiBus(&spi_bus_handle);
    Board::spi_bus_1->init();
    Board::spi1 = new Driver::Spi(Board::spi_bus_1, &spi1_handle);
    Board::spi2 = new Driver::Spi(Board::spi_bus_1, &spi2_handle);
    Board::spi1->init();
    Board::spi2->init();

    adcInit();
    Board::adc_unit = new Driver::AdConverterUnit(&adc_unit_handle);
    Board::adc_unit->init();
    Board::adc_channel0 = new Driver::AdConverterChannel(Board::adc_unit, &adc_channel0_handle);
    Board::adc_channel0->init();

    pwmHandleInit();
    Driver::Pwm::timerInit(&pwm_timer_handle);
    Board::pwm0 = new Driver::Pwm(&pwm0_handle);
    Board::pwm1 = new Driver::Pwm(&pwm1_handle);
    Board::pwm2 = new Driver::Pwm(&pwm2_handle);
    Board::pwm3 = new Driver::Pwm(&pwm3_handle);
    Board::pwm0->init();
    Board::pwm1->init();
    Board::pwm2->init();
    Board::pwm3->init();

    udpHandleInit();
    Board::udp = new Driver::Udp(&udp_handle, UDP_PORT_ID);
    Board::udp->init();

    wlanHandleInit();
    Driver::WlanDriver::init(&wlan_handle);
}

void eventHandle(void *param);

void deviceInit()
{
    (new Component::Icm42688(Board::spi1))->init();
    (new Component::Dps310(Board::spi2))->init();
    (new Component::BuildInBattery(Board::adc_channel0))->init();

    (new Framework::Accelerometer("acc"))->bind(Framework::PhysicalDevice::find("ICM42688"));
    (new Framework::Gyroscope("gyro"))->bind(Framework::PhysicalDevice::find("ICM42688"));
    (new Framework::Altimeter("altimeter"))->bind(Framework::PhysicalDevice::find("DPS310"));
    (new Framework::Voltmeter("voltmeter"))->bind(Framework::PhysicalDevice::find("BuildInBattery"));
    BASE_INFO("DEVICE INIT FINISH");
}
