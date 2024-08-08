//
// Created by zhaohe on 2023/8/30.
//

#ifndef THERMOMETER_H_
#define THERMOMETER_H_

#include "Device/device_data.h"
#include "Device/Virtual/virtual_device.h"

namespace Framework
{
    class Thermometer : public VirtualDevice
    {
    public:
        explicit Thermometer(const char *name);

        AC_RET read(TempData &data);

        ~Thermometer() = default;
    };
}

#endif //THERMOMETER_H_
