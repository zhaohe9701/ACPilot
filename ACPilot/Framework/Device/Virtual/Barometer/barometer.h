//
// Created by zhaohe on 2024/4/17.
//

#ifndef BAROMETER_H_
#define BAROMETER_H_

#include "Device/device_data.h"
#include "Device/Virtual/virtual_device.h"

namespace Framework
{
    class Barometer : public VirtualDevice
    {
    public:

        explicit Barometer(const char *name);

        AC_RET read(AtaData &data);

        ~Barometer() = default;
    };
}
#endif //BAROMETER_H_
