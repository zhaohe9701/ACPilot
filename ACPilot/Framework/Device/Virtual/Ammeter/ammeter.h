//
// Created by zhaohe on 2024/5/11.
//

#ifndef AMMETER_H_
#define AMMETER_H_


#include "Device/Virtual/virtual_device.h"

class Ammeter : public VirtualDevice
{
public:
    explicit Ammeter(const char *name);

    AC_RET read(CurData &data);

    ~Ammeter() = default;
};


#endif //AMMETER_H_
