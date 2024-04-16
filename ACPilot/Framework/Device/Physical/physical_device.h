//
// Created by zhaohe on 2024/4/12.
//

#ifndef PHYSICAL_DEVICE_H_
#define PHYSICAL_DEVICE_H_

#include "Device/device_data.h"

class PhysicalDevice
{
public:
    PhysicalDevice() = default;

    virtual AC_RET readAccelerometer(AccData &data);

    virtual AC_RET readGyroscope(GyroData &data);

    virtual AC_RET readMagnetometer(MagData &data);

    virtual AC_RET readThermometer(TempData &data);

    virtual AC_RET readAltimeter(AltitudeData &data);

    virtual AC_RET readBarometer(AtaData &data);

    bool haveAbility(VirtualDeviceType type);

    ~PhysicalDevice() = default;

private:
    DeviceAbility _ability = 0x0;
};

#endif //PHYSICAL_DEVICE_H_
