//
// Created by zhaohe on 2023/8/30.
//

#ifndef MAGNETOMETER_H_
#define MAGNETOMETER_H_

#include "Device/device_data.h"
#include "Device/Virtual/virtual_device.h"

class Magnetometer : public VirtualDevice
{
public:
    explicit Magnetometer(const char *name);

    AC_RET read(MagData &data);

    ~Magnetometer() = default;
};


#endif //MAGNETOMETER_H_
