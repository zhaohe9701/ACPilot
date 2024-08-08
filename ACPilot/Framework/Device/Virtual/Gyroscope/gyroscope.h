//
// Created by zhaohe on 2023/8/30.
//

#ifndef GYROSCOPE_H_
#define GYROSCOPE_H_

#include "Device/device_data.h"
#include "Device/Virtual/virtual_device.h"

namespace Framework
{
    class Gyroscope : public VirtualDevice
    {
    public:
        explicit Gyroscope(const char *name);

        AC_RET read(GyroData &data);

        AC_RET cali(Common::Vec3 *data, uint16_t num, DeviceCali *cali) override;

        AC_RET copyRot(DeviceCaliData *cali);

        AC_RET clearCali() override;

        DeviceCaliData *getCali() override;

        AC_RET setCali(DeviceCaliData &cali) override;

        ~Gyroscope() = default;

    private:
        DeviceCaliData _cali_data;
    };
}

#endif //GYROSCOPE_H_
