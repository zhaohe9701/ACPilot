//
// Created by zhaohe on 2024/4/12.
//

#ifndef PHYSICAL_DEVICE_H_
#define PHYSICAL_DEVICE_H_

#include "Device/device_data.h"
#include "ac_list.h"
#include "error_handing.h"
#include "default_debug.h"

class PhysicalDevice
{
public:
    PhysicalDevice();

    bool match(const char *name);

    virtual AC_RET readAccelerometer(AccData &data);

    virtual AC_RET readGyroscope(GyroData &data);

    virtual AC_RET readMagnetometer(MagData &data);

    virtual AC_RET readThermometer(TempData &data);

    virtual AC_RET readAltimeter(AltitudeData &data);

    virtual AC_RET readBarometer(AtaData &data);

    virtual AC_RET readLocator(PosData &data);

    bool haveAbility(VirtualDeviceType type);

    static void add(PhysicalDevice *device);

    static PhysicalDevice *find(const char *name);

    ~PhysicalDevice() = default;

protected:
    DeviceAbility _ability = 0x0;
    char _name[PARAM_NAME_LEN] = "unknown";
private:
    static List<PhysicalDevice *> _list;

};

#endif //PHYSICAL_DEVICE_H_
