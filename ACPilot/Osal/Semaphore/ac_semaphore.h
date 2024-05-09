/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2023-03-26 22:14:49
 * @LastEditors: zhaohe
 * @LastEditTime: 2023-03-26 22:27:52
 * @FilePath: \ZH_FLIGHT\Sys\OS\ac_semaphore.h
 * Copyright (C) 2023 zhaohe. All rights reserved.
 */
#ifndef AC_SEMAPHORE_H_
#define AC_SEMAPHORE_H_

#include "os.h"
#include "type.h"

class AcBinSemaphore
{
public:
    AcBinSemaphore();

    AC_RET get(uint32_t timeout = AC_FOREVER);

    AC_RET give();

private:
    SemaphoreHandle _handle = nullptr;
};

class AcCountSemaphore
{
private:
    SemaphoreHandle_t _handle = nullptr;
public:
    explicit AcCountSemaphore(uint32_t max_count, uint32_t init_count = 0);

    AC_RET get(uint32_t timeout = AC_FOREVER);

    AC_RET give();
};

#endif
