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

#include "type.h"
#include "Interface/com_interface.h"
#include "Thread/ac_thread.h"

#define USB_EVENT_TASK_PRIO 12
class Usb : virtual public ComInterface
{
    friend class UsbManager;
public:
    explicit Usb(uint8_t mark);
    AC_RET init();
    AC_RET send(uint8_t* buf, uint16_t length, uint32_t timeout) override;
    AC_RET open() override;
private:
    uint32_t _ptr = 0;
#ifdef C_ESP32
    QueueHandle _queue_handle = nullptr;
    AcThread _task{nullptr, 256, USB_EVENT_TASK_PRIO};
#endif
};

class UsbManager
{
public:
    static void add(Usb *usb);

#ifdef C_STM32
    static void usbCallbackHandle(uint8_t *buf, uint32_t len);
#endif
#ifdef C_ESP32
    static void usbReceiveHandle(void *param);
#endif
private:
    static Usb *_usb;
};

#endif //USB_DRIVER_H_
