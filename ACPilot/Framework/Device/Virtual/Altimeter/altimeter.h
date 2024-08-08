//
// Created by zhaohe on 2024/4/17.
//

#ifndef ALTIMETER_H_
#define ALTIMETER_H_

#include "Device/device_data.h"
#include "Device/Virtual/virtual_device.h"

namespace Framework
{

    class Altimeter : public VirtualDevice
    {
    public:

        explicit Altimeter(const char *name);

        AC_RET read(AltitudeData &data);

        ~Altimeter() = default;
    };
}
#endif //ALTIMETER_H_
