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

#include "IO/device_interface.h"
#include "driver/spi_master.h"
#include "Gpio/gpio_driver.h"
#include "ac_list.h"
#include "Semaphore/ac_semaphore.h"
#include "Mutex/ac_mutex.h"
#include <string.h>

struct SpiBusHandle
{
    SpiBusHandle();
    spi_bus_config_t config{};
    spi_host_device_t host = SPI1_HOST;
    spi_common_dma_t dma = SPI_DMA_DISABLED;
};



struct SpiHandle
{
    SpiHandle();
    spi_device_interface_config_t config{};
    spi_device_handle_t handle{};
};

#endif