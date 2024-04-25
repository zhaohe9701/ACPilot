//
// Created by zhaohe on 2023/8/29.
//

#ifndef DEVICE_H_
#define DEVICE_H_

#include "Device/device_data.h"
#include "Device/device_cali.h"
#include "Device/Physical/physical_device.h"
#include "ac_list.h"
#include "error_handing.h"

#define DEVICE_NAME_LEN PARAM_NAME_LEN

class VirtualDevice
{
public:
    explicit VirtualDevice(const char *name);

    AC_RET bind(PhysicalDevice *device);

    bool isMatch(const char *name, VirtualDeviceType type);

    virtual AC_RET cali(Vec3 *data, uint16_t num, DeviceCali *cali);

    virtual AC_RET clearCali();

    virtual DeviceCaliData *getCali();

    virtual AC_RET setCali(DeviceCaliData &cali);

    ~VirtualDevice() = default;

    static void add(VirtualDevice *device);

    static VirtualDevice *find(const char *name, VirtualDeviceType type);

protected:
    char _name[DEVICE_NAME_LEN] = "unknown";
    VirtualDeviceType _type = DEFAULT_DEV;
    PhysicalDevice *_physical_device = nullptr;

    static List<VirtualDevice *> _list;

};


#endif //DEVICE_H_
