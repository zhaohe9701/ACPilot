#ifndef ACTUATOR_DATA_H_
#define ACTUATOR_DATA_H_

#include "config.h"

class ActuatorData
{
public:
    float motor[MOTOR_NUM];
    float servo[SERVO_NUM];
};

#endif