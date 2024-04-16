//
// Created by zhaohe on 2023/8/29.
//

#ifndef ACCELEROMETER_H_
#define ACCELEROMETER_H_

#include "Device/device_data.h"
#include "Device/Virtual/virtual_device.h"

class Accelerometer : virtual public VirtualDevice
{
public:
    explicit Accelerometer(const char *name);

    AC_RET read(AccData &data);

    ~Accelerometer() = default;
};


#endif //ACCELEROMETER_H_
