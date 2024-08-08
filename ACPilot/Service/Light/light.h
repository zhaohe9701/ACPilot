//
// Created by zhaohe on 2024/4/19.
//

#ifndef LIGHT_H_
#define LIGHT_H_

#include "Type/type.h"

namespace Service
{
    enum LightMode
    {
        LIGHT_KEEP_OFF,
        LIGHT_KEEP_ON,
        LIGHT_SLOW_FLASHING,
        LIGHT_FAST_FLASHING,
        LIGHT_BREATHE,
        LIGHT_PULSE_FLASHING,
        LIGHT_DOUBLE_PULSE_FLASHING,
    };

    class Light
    {
    public:
        explicit Light(uint8_t id);

        bool match(uint8_t id);

        virtual AC_RET setMode(LightMode mode);

    protected:
        uint8_t _id;
    };
}

#endif //LIGHT_H_
