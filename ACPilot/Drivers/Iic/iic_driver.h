//
// Created by zhaohe on 2023/10/17.
//

#ifndef IIC_DRIVER_H_
#define IIC_DRIVER_H_

#ifdef C_STM32
#include "STM32/iic_driver_param.h"
#endif

#ifdef C_ESP32

#include "ESP32/iic_driver_param.h"

#endif

#include "Type/type.h"
#include "Mutex/mutex.h"
#include "List/ac_list.h"
#include "Io/io.h"

namespace Driver
{
    class IicBus
    {
        friend class IicBusManager;

    public:
        explicit IicBus(IicBusHandle *handle);

        AC_RET init() const;

        bool matchHandle(IicBusHandle *handle) const;

        AC_RET lock(uint32_t timeout = AC_FOREVER);

        void unlock();

        IicBusHandle *handle = nullptr;
    protected:
#ifdef C_STM32
        AC_RET waitWriteFinish(uint32_t timeout = AC_FOREVER);
        AC_RET waitReadFinish(uint32_t timeout = AC_FOREVER);
        void writeFinishNotify();
        void readFinishNotify();
#endif
    private:
#ifdef C_STM32
        AcBinSemaphore _read_sem;
        AcBinSemaphore _write_sem;
#endif
        Osal::Mutex _mutex;
    };

    class Iic : public Interface::IO
    {
    public:
        Iic(IicBus *bus, IicHandle *handle);

        AC_RET init();

        AC_RET readReg(uint8_t address, uint8_t &value, uint32_t timeout) override;

        AC_RET readBytes(uint8_t address, uint8_t len, uint8_t *dataBuf, uint32_t timeout) override;

        AC_RET readBytesDMA(uint8_t address, uint8_t len, uint8_t *dataBuf, uint32_t timeout) override;

        AC_RET writeReg(uint8_t address, uint8_t value, uint32_t timeout) override;

        AC_RET writeBytes(uint8_t address, uint8_t len, uint8_t *value, uint32_t timeout) override;

        AC_RET writeBytesDMA(uint8_t address, uint8_t len, uint8_t *value, uint32_t timeout) override;

    private:
        IicBus *_bus = nullptr;
        IicHandle *_handle = nullptr;
    };
}

#endif //IIC_DRIVER_H_
