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

    AC_RET cali(Vec3 *data, uint16_t num, DeviceCali *cali) override;

    AC_RET clearCali() override;

    ~Magnetometer() = default;
private:
    DeviceCaliData _cali_data;
};


#endif //MAGNETOMETER_H_
