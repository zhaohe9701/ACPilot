/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2023-03-26 21:11:21
 * @LastEditors: zhaohe
 * @LastEditTime: 2023-03-26 21:35:47
 * @FilePath: \ZH_FLIGHT\Sys\sys.h
 * Copyright (C) 2023 zhaohe. All rights reserved.
 */
#ifndef SYS_H_
#define SYS_H_

#include "Aircraft/aircraft.h"
#include "Receive/receive_server.h"
#include "Semaphore/ac_semaphore.h"
#include "Command/command_server.h"
#include "Spi/spi_driver.h"
#include "Transmit/transmit_server.h"
#include "Mail/mailbox.h"
#include "Debug/print.h"

class Sys
{
public:
    /* 飞行器实例 */
    static Aircraft *aircraft;
};
#endif
