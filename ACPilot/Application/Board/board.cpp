//
// Created by zhaohe on 2023/8/16.
//
#include "board.h"
#include "Uart/uart_driver.h"
#include "Sensor/Imu/icm42688.h"
#include "Gpio/gpio_driver.h"
#include "Usb/usb_driver.h"
#include "Wlan/wlan_driver.h"

/* uart */
UartHandle esp32_mini_uart1_handle;
UartHandle esp32_mini_uart2_handle;
/* wlan */
WlanHandle wlan_handle;
/* udp */
UdpHandle udp_handle;

void uartHandleInit()
{
    esp32_mini_uart1_handle.index = UART_NUM_1;
    esp32_mini_uart1_handle.config.baud_rate = 921600;
    esp32_mini_uart1_handle.config.data_bits = UART_DATA_8_BITS;
    esp32_mini_uart1_handle.config.parity = UART_PARITY_DISABLE;
    esp32_mini_uart1_handle.config.stop_bits = UART_STOP_BITS_1;
    esp32_mini_uart1_handle.config.flow_ctrl = UART_HW_FLOWCTRL_DISABLE;
    esp32_mini_uart1_handle.config.source_clk = UART_SCLK_DEFAULT;
    esp32_mini_uart1_handle.tx = GPIO_NUM_17;
    esp32_mini_uart1_handle.rx = GPIO_NUM_18;

    esp32_mini_uart2_handle.index = UART_NUM_2;
    esp32_mini_uart2_handle.config.baud_rate = 921600;
    esp32_mini_uart2_handle.config.data_bits = UART_DATA_8_BITS;
    esp32_mini_uart2_handle.config.parity = UART_PARITY_DISABLE;
    esp32_mini_uart2_handle.config.stop_bits = UART_STOP_BITS_1;
    esp32_mini_uart2_handle.config.flow_ctrl = UART_HW_FLOWCTRL_DISABLE;
    esp32_mini_uart2_handle.config.source_clk = UART_SCLK_DEFAULT;
    esp32_mini_uart2_handle.tx = GPIO_NUM_35;
    esp32_mini_uart2_handle.rx = GPIO_NUM_36;
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

Gpio *Board::imu_cs_pin = nullptr;
Gpio *Board::baro_cs_pin = nullptr;
Gpio *Board::flash_cs_pin = nullptr;
Gpio *Board::led_pin = nullptr;
Gpio *Board::imu_interrupt_pin = nullptr;
SpiBus *Board::spi1_bus = nullptr;
Uart *Board::uart1 = nullptr;
Uart *Board::uart2 = nullptr;
Usb *Board::usb = nullptr;
Udp *Board::udp = nullptr;
ExtInterrupt *Board::imu_interrupt = nullptr;

void test(void *param);

void boardInit()
{

    Board::usb = new Usb(0x01);
    Board::usb->init();

    uartHandleInit();
    Board::uart1 = new Uart(&esp32_mini_uart1_handle, 0x02);
    Board::uart1->init();
    Board::uart2 = new Uart(&esp32_mini_uart2_handle, 0x03);
    Board::uart2->init();

    udpHandleInit();
    Board::udp = new Udp(&udp_handle, 0x04);
    Board::udp->init();

    wlanHandleInit();
    WlanDriver::init(&wlan_handle);

    AcThread *test_thread = new AcThread("test", 4096, 24);
    test_thread->run(test, nullptr);
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

#include "Json/ac_json.h"
#include "DataModule/data_module_framework.h"
#include "MemoryPool/memory_pool_framework.h"
#include "Thread/thread_framework.h"
#include "Memory/ac_memory.h"

#pragma pack(1)

typedef struct
{
    char name[10];
    int8_t weight;
    struct
    {
        int8_t l;
    } size[5];
    uint32_t max_speed;
    uint8_t remote;
} Aircraft;

#pragma pack()

void test(void *param)
{
    printf("test\n");
    char buf[2048] = R"({"aircraft":{"name":"string[10]","weight":"int8","size[5]":[{"l":"int8"}],"max_speed":"uint32","remote":"uint8"}})";
//    JsonTree *tree = Json::deserialize(buf);
//    if (nullptr == tree)
//    {
//        printf("deserialize error\n");
//    } else
//    {
//        printf("deserialize success\n");
//    }
//    memset(buf, 0, 2048);
//    Json::serialize(tree, buf, 2048);
//    printf("serialize: %s\n", buf);
//
    DataModuleFramework::create(buf);
//
//    printf("create finish\n");
//
//    Json::free(tree);
//
//
//    memset(buf, 0, 2048);
//
//    if (AC_OK != DataModule::dumpStruct(buf, 2048))
//    {
//        printf("dumpStruct error\n");
//    }
//
//    printf("dataModule Struct: %s\n", buf);
//
//    memset(buf, 0, 2048);
//
//    MemoryPoolManager::info(buf, 2048);
//
//    printf("info: %s\n", buf);
//
//    memset(buf, 0, 2048);
//
//    DataModule::get("/aircraft", buf, 2048);
//
//    printf("get: %s\n", buf);
//
//    DataModule::set(R"(/aircraft)", R"({"aircraft":{"name":"111111","weight":"10"}})");
//
//    DataModule::add(R"(/aircraft/size[5])", R"({"size[5]":[{"l":"10"},{"l":"20"}]})");
//
//    DataModule::del(R"(/aircraft/size[5]/1)");
//
//    memset(buf, 0, 2048);
//    DataModule::dumpData(buf, 2048);
//
//    printf("dataModule Data: %s\n", buf);
//
//    memset(buf, 0, 2048);
//
//    DataModule::info(buf, 2048);
//
//    printf("%s\n", buf);
//
//    printf("size: %d\n", sizeof(DataTree));
//
//    Aircraft aircraft;
//
//    DataModule::read(R"(/aircraft)", &aircraft, sizeof(Aircraft));
//
//    printf("name: %s, weight: %d, size: %d, max_speed: %lu, remote: %d\n", aircraft.name, aircraft.weight, aircraft.size[0].l, aircraft.max_speed, aircraft.remote);
//
//    aircraft.weight = 20;
//
//    DataModule::write(R"(/aircraft)", &aircraft, sizeof(Aircraft));
//
//    memset(buf, 0, 2048);
//    DataModule::dumpData(buf, 2048);
//
//    printf("dataModule Data: %s\n", buf);
//
    memset(buf, 0, 2048);

    MemoryPoolManager::info(buf, 2048);

    printf("%s\n", buf);
//
//    tickSleep(1000);
//    memset(buf, 0, 2048);
//
//    ThreadManager::info(buf, 2048);
//
//    printf("%s\n", buf);

    printf("Memory size:now:%lu min:%lu\n", Memory::getFreeHeapSize(), Memory::getMinimumFreeHeapSize());
    printf("sp_get_free_internal_heap_size = %ld\n\r", esp_get_free_internal_heap_size());

    while (1)
    {
        tickSleep(10000);
    }
}


