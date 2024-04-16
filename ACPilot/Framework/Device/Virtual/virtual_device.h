//
// Created by zhaohe on 2023/8/29.
//

#ifndef DEVICE_H_
#define DEVICE_H_

#include "Device/device_data.h"
#include "Device/Physical/physical_device.h"
#include "ac_list.h"

#define DEVICE_NAME_LEN PARAM_NAME_LEN

class VirtualDevice
{
public:
    explicit VirtualDevice(const char *name);

    AC_RET bind(PhysicalDevice *device);

    bool isMatch(const char *name, VirtualDeviceType type);

    ~VirtualDevice() = default;

    static VirtualDevice *find(const char *name, VirtualDeviceType type);

protected:
    char _name[DEVICE_NAME_LEN] = {0};
    VirtualDeviceType _type = DEFAULT_DEV;
    PhysicalDevice *_physical_device = nullptr;

    static List<VirtualDevice *> _list;

};


#endif //DEVICE_H_
