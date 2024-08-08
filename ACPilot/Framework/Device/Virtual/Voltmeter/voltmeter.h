//
// Created by zhaohe on 2024/5/11.
//

#ifndef VOLTMETER_H_
#define VOLTMETER_H_


#include "Device/Virtual/virtual_device.h"

namespace Framework
{
    class Voltmeter : public VirtualDevice
    {
    public:
        explicit Voltmeter(const char *name);

        AC_RET read(VolData &data);

        ~Voltmeter() = default;
    };
}

#endif //VOLTMETER_H_
