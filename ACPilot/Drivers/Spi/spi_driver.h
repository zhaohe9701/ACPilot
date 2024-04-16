//
// Created by zhaohe on 2023/10/17.
//

#ifndef SPI_DRIVER_H_
#define SPI_DRIVER_H_

#ifdef C_STM32
#include "STM32/spi_driver_param.h"
#endif

#ifdef C_ESP32

#include "ESP32/spi_driver_param.h"

#endif

class SpiBus
{
    friend class SpiBusManager;

public:
    explicit SpiBus(SpiBusHandle *handle);

    AC_RET init() const;

    bool matchHandle(SpiBusHandle *handle) const;

    AC_RET lock(uint32_t timeout = AC_FOREVER);

    void unlock();

    SpiBusHandle *handle = nullptr;
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
    AcMutex _mutex;
};

class Spi : public IoInterface
{
public:
    Spi(SpiBus *bus, SpiHandle *handle);

    AC_RET init();

    AC_RET readReg(uint8_t address, uint8_t &value, uint32_t timeout) override;

    AC_RET readBytes(uint8_t address, uint8_t len, uint8_t *dataBuf, uint32_t timeout) override;

    AC_RET readBytesDMA(uint8_t address, uint8_t len, uint8_t *dataBuf, uint32_t timeout) override;

    AC_RET writeReg(uint8_t address, uint8_t value, uint32_t timeout) override;

    AC_RET writeBytes(uint8_t address, uint8_t len, uint8_t *value, uint32_t timeout) override;

    AC_RET writeBytesDMA(uint8_t address, uint8_t len, uint8_t *value, uint32_t timeout) override;

private:
    SpiHandle *_handle = nullptr;
    SpiBus *_bus = nullptr;
#ifdef C_STM32
    void _enable();
    void _disable();
#endif
};

#endif //SPI_DRIVER_H_
