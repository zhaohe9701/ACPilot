/*
 * @Description:
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-09-27 22:21:06
 * @LastEditors: zhaohe
 * @LastEditTime: 2023-02-25 02:06:27
 * @FilePath: \ZH_FLIGHT\Sys\Driver\z_spi.h
 * Copyright (C) 2022 zhaohe. All rights reserved.
 */
#ifndef SPI_DRIVER_PARAM_H_
#define SPI_DRIVER_PARAM_H_

#include "Interface/device_interface.h"
#include "spi.h"
#include "Gpio/gpio_driver.h"
#include "ac_list.h"
#include "Semaphore/ac_semaphore.h"
#include "Mutex/ac_mutex.h"

#define SPI_PRESCALER_2     SPI_BAUDRATEPRESCALER_2
#define SPI_PRESCALER_4     SPI_BAUDRATEPRESCALER_4
#define SPI_PRESCALER_8     SPI_BAUDRATEPRESCALER_8
#define SPI_PRESCALER_16    SPI_BAUDRATEPRESCALER_16
#define SPI_PRESCALER_32    SPI_BAUDRATEPRESCALER_32
#define SPI_PRESCALER_64    SPI_BAUDRATEPRESCALER_64
#define SPI_PRESCALER_128   SPI_BAUDRATEPRESCALER_128
#define SPI_PRESCALER_256   SPI_BAUDRATEPRESCALER_256


typedef SPI_TypeDef SpiBusSelect;
typedef SPI_HandleTypeDef SpiBusHandle;

#endif