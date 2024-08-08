//
// Created by zhaohe on 2023/10/17.
//

#ifndef UART_DRIVER_H_
#define UART_DRIVER_H_

#ifdef C_STM32
#include "STM32/uart_driver_param.h"
#endif

#ifdef C_ESP32

#include "ESP32/uart_driver_param.h"

#endif

#include "Thread/thread.h"
#include "Com/com.h"
#include "Mutex/mutex.h"
#include "HardTimer/hard_timer_driver.h"

namespace Driver
{
    class Uart : virtual public Interface::Com
    {
    public:

        /* 使用handle初始化 */
        Uart(UartHandle *handle, uint8_t port_num);

        /* 使用handle初始化 */
        AC_RET init() override;

        /* 获取handle */
        UartHandle *getHandle();

        /* 打开端口 */
        AC_RET open() override;

        /* 通过端口发包 */
        AC_RET send(uint8_t *buf, uint16_t length, uint32_t timeout) override;

        /* 匹配handle */
        bool match(UartHandle *handle);

        bool match(uint8_t port_num) override;

    private:
        UartHandle *_handle = nullptr;
#ifdef C_STM32
        /* 发送传输完成信号 */
        void sendFinish();
        /* 等待传输完成 */
        void waitSendFinish(uint32_t timeout);
        AcBinSemaphore _sem;
#endif
#ifdef C_ESP32

        void _check();

        static void _receive_task(void *param);

        static void _timer_callback(void *param);

        HardwareTimer *_timer = nullptr;
        Osal::Queue<uart_event_t> *_event_queue = nullptr;
        Osal::AcThread *_uart_task = nullptr;
#endif
    };
}
#endif //UART_DRIVER_H_
