//
// Created by zhaohe on 2023/8/30.
//

#ifndef LOCATOR_H_
#define LOCATOR_H_

#include "Device/device_data.h"
#include "Device/Virtual/virtual_device.h"

namespace Framework
{
    class Locator : public VirtualDevice
    {
    public:
        explicit Locator(const char *name);

        AC_RET read(PosData &data);

        ~Locator() = default;
    };
}
#endif //LOCATOR_H_
