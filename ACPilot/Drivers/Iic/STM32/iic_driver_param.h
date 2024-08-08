/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-09-27 22:23:50
 * @LastEditors: zhaohe
 * @LastEditTime: 2023-02-25 02:06:12
 * @FilePath: \ZH_FLIGHT\Sys\Driver\z_iic.h
 * Copyright (C) 2022 zhaohe. All rights reserved.
 */
#ifndef IIC_DRIVER_PARAM_H_
#define IIC_DRIVER_PARAM_H_

#include "Interface/device_interface.h"
#include "i2c.h"
#include "Gpio/gpio_driver.h"
#include "List/ac_list.h"
#include "Semaphore/semaphore.h"
#include "Mutex/mutex.h"

#define IIC_100KHZ  0x10C0ECFF
#define IIC_400KHZ  0x009034B6
#define IIC_1000KHZ 0x00401242

typedef I2C_TypeDef IicBusSelect;
typedef I2C_HandleTypeDef IicBusHandle;

class IicBus
{
    friend class IicBusManager;

public:
    IicBus();

    explicit IicBus(IicBusHandle *handle);

    IicBus(Gpio *sda, Gpio *scl);

    AC_RET init(IicBusSelect *iic_name, uint32_t frq);

    AC_RET initAdvanced();

    bool matchHandle(IicBusHandle *hdle);

    AC_RET waitWirteFinish(uint32_t timeout = AC_FOREVER);

    AC_RET waitReadFinish(uint32_t timeout = AC_FOREVER);

    AC_RET lock(uint32_t timeout = AC_FOREVER);

    void unlock();

    IicBusHandle *handle = nullptr;
protected:
    void writeFinishNotify();

    void readFinishNotify();

private:
    Gpio *_sda = nullptr;
    Gpio *_scl = nullptr;
    AcBinSemaphore _read_sem;
    AcBinSemaphore _write_sem;
    AcMutex _mutex;
};

class IicBusManager
{
public:
    static void add(IicBus *Iic_bus);

    static IicBus *find(IicBusHandle *handle);

    static void writeFinishHandle(IicBusHandle *handle);

    static void readFinishHandle(IicBusHandle *handle);

private:
    static List<IicBus *> _list;
};


#endif