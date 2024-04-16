//
// Created by zhaohe on 2023/8/30.
//

#ifndef GYROSCOPE_H_
#define GYROSCOPE_H_

#include "Device/device_data.h"
#include "Device/Virtual/virtual_device.h"

class Gyroscope : public VirtualDevice
{
public:
    explicit Gyroscope(const char *name);

    AC_RET read(GyroData &data);

    ~Gyroscope() = default;
};


#endif //GYROSCOPE_H_
