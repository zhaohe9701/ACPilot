//
// Created by zhaohe on 2024/4/12.
//

#ifndef PHYSICAL_DEVICE_H_
#define PHYSICAL_DEVICE_H_

#include "Device/device_data.h"
#include "List/ac_list.h"
#include "error_handing.h"
#include "Debug/default_debug.h"

namespace Framework
{
    class PhysicalDevice
    {
    public:
        PhysicalDevice();

        bool match(const char *name);

        virtual AC_RET readAccelerometer(AccData &data);            // 加速度计

        virtual AC_RET readGyroscope(GyroData &data);               // 陀螺仪

        virtual AC_RET readMagnetometer(MagData &data);             // 磁力计

        virtual AC_RET readThermometer(TempData &data);             // 温度计

        virtual AC_RET readAltimeter(AltitudeData &data);           // 高度计

        virtual AC_RET readBarometer(AtaData &data);                // 气压计

        virtual AC_RET readLocator(PosData &data);                  // 定位器

        virtual AC_RET readVoltmeter(VolData &data);                // 电压计

        virtual AC_RET readAmmeter(CurData &data);                  // 安培表

        virtual AC_RET readCoulometer(ElectricQuantityData &data);  // 库伦计

        bool haveAbility(VirtualDeviceType type);

        static void add(PhysicalDevice *device);

        static PhysicalDevice *find(const char *name);

        ~PhysicalDevice() = default;

    protected:
        DeviceAbility _ability = 0x0;
        char _name[PARAM_NAME_LEN] = "unknown";
    private:
        static Common::List<PhysicalDevice *> _list;

    };
}
#endif //PHYSICAL_DEVICE_H_
