//
// Created by zhaohe on 2023/8/30.
//

#ifndef MAGNETOMETER_H_
#define MAGNETOMETER_H_

#include "device.h"
#include "type.h"

typedef Vec3 MagData;

class Magnetometer : public Device
{
public:
    Magnetometer();
    void setName(const char *name);
    AC_RET read(MagData &data);
    ~Magnetometer() = default;
protected:
    virtual AC_RET readMag(MagData &data) = 0;
};


#endif //MAGNETOMETER_H_
