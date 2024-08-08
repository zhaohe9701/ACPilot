//
// Created by zhaohe on 2024/5/11.
//

#ifndef BATTERY_H_
#define BATTERY_H_


#include "Device/Physical/physical_device.h"

namespace Framework
{
class Battery : public PhysicalDevice
{
public:
    Battery() = default;

    virtual AC_RET init();

    virtual AC_RET getVoltage(VolData &data);

    virtual AC_RET getCurrent(CurData &data);

    virtual AC_RET getElectricQuantity(ElectricQuantityData &data);

    virtual AC_RET updateVoltage();

    virtual AC_RET updateCurrent();

    virtual AC_RET updateElectricQuantity();

    AC_RET readVoltmeter(VolData &data) override;

    AC_RET readAmmeter(CurData &data) override;

    AC_RET readCoulometer(ElectricQuantityData &data) override;

protected:
    VolData _vol_data;
    CurData _cur_data;
    ElectricQuantityData _electric_quantity_data;
};

}

#endif //BATTERY_H_
