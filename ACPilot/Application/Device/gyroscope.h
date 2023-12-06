//
// Created by zhaohe on 2023/8/30.
//

#ifndef GYROSCOPE_H_
#define GYROSCOPE_H_

#include "device.h"
#include "type.h"

typedef Vec3 GyroData;

class Gyroscope : public Device
{
public:
    Gyroscope();
    void setName(const char *name);
    AC_RET read(GyroData &data);
    ~Gyroscope() = default;
protected:
    virtual AC_RET readGyro(GyroData &data) = 0;
};


#endif //GYROSCOPE_H_
