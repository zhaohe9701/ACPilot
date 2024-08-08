//
// Created by zhaohe on 2024/4/23.
//

#ifndef PWM_MOTOR_H_
#define PWM_MOTOR_H_

#include "Motor/motor.h"
#include "Pwm/pwm_driver.h"

namespace Component
{
    class PwmMotor : public Motor
    {
    public:
        PwmMotor(Driver::Pwm *pwm, uint8_t id);

        AC_RET set(float value) override;

    private:
        Driver::Pwm *_pwm = nullptr;
    };
}

#endif //PWM_MOTOR_H_
