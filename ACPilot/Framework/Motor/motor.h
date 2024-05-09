//
// Created by zhaohe on 2024/4/23.
//

#ifndef MOTOR_H_
#define MOTOR_H_


#include "type.h"

#define MAX_MOTOR_NUM 4

class Motor
{
public:
    explicit Motor(uint8_t id);

    virtual AC_RET init();

    virtual AC_RET set(float value) = 0;

    virtual AC_RET enable();

    virtual AC_RET disable();

    ~Motor() = default;

    static AC_RET set(uint8_t id, float value);

    static void stopAll();

    static void enableAll();

    static void disableAll();

protected:
    uint8_t _id = 0x0;
    static Motor *_group[MAX_MOTOR_NUM];
};


#endif //MOTOR_H_
