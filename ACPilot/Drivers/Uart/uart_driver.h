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

#include "Thread/ac_thread.h"
#include "Interface/com_interface.h"
#include "Mutex/ac_mutex.h"

#define UART_MANAGER_TASK_PRIO 12

class Uart : virtual public ComInterface
{
    friend class UartManager;
public:

    /* 使用handle初始化 */
    Uart(UartHandle *handle, uint8_t port_num);
    /* 使用handle初始化 */
    AC_RET init();

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
    UartHandle *handle = nullptr;
#ifdef C_STM32
    /* 发送传输完成信号 */
    void sendFinish();
    /* 等待传输完成 */
    void waitSendFinish(uint32_t timeout);
    AcBinSemaphore _sem;
#endif
#ifdef C_ESP32
    void check();
    uint32_t tmp_len = 0;
#endif
};

class UartManager
{
public:
    static void add(Uart *uart);
    static Uart* find(UartHandle *handle);
    static Uart* find(uint8_t port_num);
#ifdef C_STM32
    static void receiveFinishHandle(UartHandle *handle, uint16_t size);
    static void uartRestartHandle(UartHandle *handle);
    static void transmitFinishHandle(UartHandle *handle);
#endif
#ifdef C_ESP32
    static bool _is_running;
    static AcMutex _mutex;
    static AcThread _task;
    static void managerTask(void *pvParameters);
#endif

private:
    static List<Uart*> _list;
};

#endif //UART_DRIVER_H_
