//
// Created by zhaohe on 2023/8/29.
//

#ifndef ACCELEROMETER_H_
#define ACCELEROMETER_H_

#include "type.h"
#include "device.h"

typedef Vec3 AccData;

class Accelerometer : virtual public Device
{
public:
    Accelerometer();
    void setName(const char *name);
    AC_RET read(AccData &data);
    ~Accelerometer() = default;
protected:
    virtual AC_RET readAcc(AccData &data) = 0;
};


#endif //ACCELEROMETER_H_
