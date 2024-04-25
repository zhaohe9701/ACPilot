//
// Created by zhaohe on 2023/8/29.
//

#ifndef ACCELEROMETER_H_
#define ACCELEROMETER_H_

#include "Device/device_data.h"
#include "Device/Virtual/virtual_device.h"

class Accelerometer : public VirtualDevice
{
public:

    explicit Accelerometer(const char *name);

    AC_RET read(AccData &data);

    AC_RET cali(Vec3 *data, uint16_t num, DeviceCali *cali) override;

    AC_RET clearCali() override;

    DeviceCaliData *getCali() override;

    AC_RET setCali(DeviceCaliData &cali) override;

    ~Accelerometer() = default;
private:
    DeviceCaliData _cali_data;
};


#endif //ACCELEROMETER_H_
