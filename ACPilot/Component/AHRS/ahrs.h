//
// Created by zhaohe on 2024/4/17.
//

#ifndef AHRS_H_
#define AHRS_H_

#include "Device/device_data.h"

namespace Component
{
    enum MemsAxisType
    {
        MEMS_6_AXIS,
        MEMS_9_AXIS,
    };

    class MemsData
    {
    public:
        explicit MemsData(MemsAxisType type)
        { _type = type; };

        MemsAxisType type()
        { return _type; };
        Framework::AccData acc;
        Framework::GyroData gyro;
        Framework::MagData mag;
    private:
        MemsAxisType _type = MEMS_6_AXIS;
    };
}
#endif //AHRS_H_
