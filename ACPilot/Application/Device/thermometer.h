//
// Created by zhaohe on 2023/8/30.
//

#ifndef THERMOMETER_H_
#define THERMOMETER_H_

#include "device.h"
#include "type.h"

typedef float TmpData;

class Thermometer : public Device
{
public:
    Thermometer();
    void setName(const char *name);
    AC_RET read(TmpData &data);
    ~Thermometer() = default;
protected:
    virtual AC_RET readTmp(TmpData &data) = 0;
};


#endif //THERMOMETER_H_
