//
// Created by zhaohe on 2023/8/29.
//

#include "virtual_device.h"
#include "string.h"
#include "Debug/default_debug.h"

using namespace Framework;

Common::List<VirtualDevice *> VirtualDevice::_list;

AC_RET VirtualDevice::bind(PhysicalDevice *device)
{
    if (nullptr == device)
    {
        BASE_ERROR("device is null");
        return AC_ERROR;
    }
    if (device->haveAbility(_type))
    {
        _physical_device = device;
        return AC_OK;
    }
    BASE_ERROR("can't find feature to support device: %s", _name);
    return AC_ERROR;
}

bool VirtualDevice::isMatch(const char *name, VirtualDeviceType type)
{
    if (0 == strncmp(_name, name, DEVICE_NAME_LEN) && _type == type)
    {
        return true;
    }
    return false;
}

VirtualDevice::VirtualDevice(const char *name)
{
    strncpy(_name, name, DEVICE_NAME_LEN - 1);
    add(this);
}

AC_RET VirtualDevice::cali(Common::Vec3 *data, uint16_t num, DeviceCali *cali)
{
    return AC_NOT_SUPPORT;
}

AC_RET VirtualDevice::clearCali()
{
    return AC_NOT_SUPPORT;
}

void VirtualDevice::add(VirtualDevice *device)
{
    _list.pushBack(device);
}

DeviceCaliData *VirtualDevice::getCali()
{
    return nullptr;
}

AC_RET VirtualDevice::setCali(DeviceCaliData &cali)
{
    return AC_NOT_SUPPORT;
}




