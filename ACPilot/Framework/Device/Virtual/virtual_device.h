//
// Created by zhaohe on 2023/8/29.
//

#ifndef DEVICE_H_
#define DEVICE_H_

#include "Device/device_data.h"
#include "Device/device_cali.h"
#include "Device/Physical/physical_device.h"
#include "List/ac_list.h"
#include "error_handing.h"

#define DEVICE_NAME_LEN PARAM_NAME_LEN

namespace Framework
{
    class VirtualDevice
    {
    public:
        explicit VirtualDevice(const char *name);

        AC_RET bind(PhysicalDevice *device);

        bool isMatch(const char *name, VirtualDeviceType type);

        virtual AC_RET cali(Common::Vec3 *data, uint16_t num, DeviceCali *cali);

        virtual AC_RET clearCali();

        virtual DeviceCaliData *getCali();

        virtual AC_RET setCali(DeviceCaliData &cali);

        ~VirtualDevice() = default;

        static void add(VirtualDevice *device);

        template<class T>
        static T *find(const char *name, VirtualDeviceType type);

    protected:
        char _name[DEVICE_NAME_LEN] = "unknown";
        VirtualDeviceType _type = DEFAULT_DEV;
        PhysicalDevice *_physical_device = nullptr;

        static Common::List<VirtualDevice *> _list;

    };

    template<class T>
    T *VirtualDevice::find(const char *name, VirtualDeviceType type)
    {
        for (Common::ListNode<VirtualDevice *> *it = _list.begin(); it != _list.end(); it = it->getNext())
        {
            if ((**it)->isMatch(name, type))
            {
                return static_cast<T *>(**it);
            }
        }
        return nullptr;
    }
}
#endif //DEVICE_H_
