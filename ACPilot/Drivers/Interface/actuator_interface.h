/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-09-27 22:31:04
 * @LastEditors: zhaohe
 * @LastEditTime: 2022-12-26 00:27:48
 * @FilePath: \ZH_FLIGHT\Sys\Motor\motor_interface.h
 * Copyright (C) 2022 zhaohe. All rights reserved.
 */
#ifndef ACTUATOR_INTERFACE_H_
#define ACTUATOR_INTERFACE_H_

#include <stdint.h>
class ActuatorInterface
{
public:
    virtual void output(uint16_t *data, uint16_t length) = 0;
    virtual void stop() = 0;
    virtual ~ActuatorInterface() = 0;
};


#endif
