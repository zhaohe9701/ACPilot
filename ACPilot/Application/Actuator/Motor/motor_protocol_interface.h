/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-12-24 20:47:31
 * @LastEditors: zhaohe
 * @LastEditTime: 2022-12-24 20:56:14
 * @FilePath: \ZH_FLIGHT\Sys\Motor\motor_protocol_interface.h
 * Copyright (C) 2022 zhaohe. All rights reserved.
 */
#ifndef MOTOR_PROTOCOL_INTERFACE_H_
#define MOTOR_PROTOCOL_INTERFACE_H_

class MotorProtocolInterface
{
public:
    virtual void setMotor(float value) = 0;
};

#endif