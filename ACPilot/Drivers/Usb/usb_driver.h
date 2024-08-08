//
// Created by zhaohe on 2023/10/17.
//

#ifndef USB_DRIVER_H_
#define USB_DRIVER_H_

#ifdef C_STM32
#include "STM32/usb_driver_param.h"
#endif

#ifdef C_ESP32

#include "STM32/usb_driver_param.h"

#endif

#include "Type/type.h"
#include "Com/com.h"
#include "Thread/thread.h"
#include "HardTimer/hard_timer_driver.h"

namespace Driver
{

    class Usb : virtual public Interface::Com
    {
    public:
        explicit Usb(uint8_t port_num);

        AC_RET init();

        AC_RET send(uint8_t *buf, uint16_t length, uint32_t timeout) override;

        AC_RET open() override;

    private:

#ifdef C_ESP32
        Osal::AcThread *_usb_task = nullptr;
        HardwareTimer *_timer = nullptr;
        uint32_t _current_len = 0;
        char _buf[MAX_USB_BUF_LEN] = {0};

        static void _receive_task(void *param);

        static void _timer_callback(void *param);

#endif
    };

}
#endif //USB_DRIVER_H_
