//
// Created by zhaohe on 2024/5/11.
//

#ifndef COULOMETER_H_
#define COULOMETER_H_


#include "Device/Virtual/virtual_device.h"

class Coulometer : public VirtualDevice
{
public:
    explicit Coulometer(const char *name);

    AC_RET read(ElectricQuantityData &data);

    ~Coulometer() = default;
};


#endif //COULOMETER_H_
