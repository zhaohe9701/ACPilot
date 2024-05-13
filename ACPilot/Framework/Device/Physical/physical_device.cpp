//
// Created by zhaohe on 2024/4/12.
//

#include <string.h>
#include "physical_device.h"

PhysicalDevice::PhysicalDevice()
{
    add(this);
}

AC_RET PhysicalDevice::readAccelerometer(AccData &data)
{
    return AC_ERROR;
}

AC_RET PhysicalDevice::readGyroscope(GyroData &data)
{
    return AC_ERROR;
}

AC_RET PhysicalDevice::readMagnetometer(MagData &data)
{
    return AC_ERROR;
}

AC_RET PhysicalDevice::readThermometer(TempData &data)
{
    return AC_ERROR;
}

AC_RET PhysicalDevice::readAltimeter(AltitudeData &data)
{
    return AC_ERROR;
}

AC_RET PhysicalDevice::readBarometer(AtaData &data)
{
    return AC_ERROR;
}

AC_RET PhysicalDevice::readLocator(PosData &data)
{
    return AC_ERROR;
}

AC_RET PhysicalDevice::readVoltmeter(VolData &data)
{
    return AC_ERROR;
}

AC_RET PhysicalDevice::readAmmeter(CurData &data)
{
    return AC_ERROR;
}

AC_RET PhysicalDevice::readCoulometer(ElectricQuantityData &data)
{
    return AC_ERROR;
}

bool PhysicalDevice::haveAbility(VirtualDeviceType type)
{
    if (0 != (_ability & ((uint32_t) 1 << type)))
    {
        return true;
    }
    return false;
}

List<PhysicalDevice *> PhysicalDevice::_list;

void PhysicalDevice::add(PhysicalDevice *device)
{
    _list.pushBack(device);
}

PhysicalDevice *PhysicalDevice::find(const char *name)
{
    for (ListNode<PhysicalDevice *> *it = _list.begin(); it != _list.end(); it = it->getNext())
    {
        if ((**it)->match(name))
        {
            return (**it);
        }
    }
    return nullptr;
}

bool PhysicalDevice::match(const char *name)
{
    if (0 == strncmp(_name, name, sizeof(_name)))
    {
        return true;
    }
    return false;
}










