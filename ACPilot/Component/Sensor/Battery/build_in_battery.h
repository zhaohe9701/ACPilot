//
// Created by zhaohe on 2024/5/11.
//

#ifndef BUILD_IN_BATTERY_H_
#define BUILD_IN_BATTERY_H_


#include "Device/Physical/Battery/battery.h"
#include "AdConverter/ad_converter_driver.h"

namespace Component
{

    class BuildInBattery : public Framework::Battery
    {
    public:
        explicit BuildInBattery(Driver::AdConverterChannel *vol_adc, Driver::AdConverterChannel *cur_adc = nullptr);

        AC_RET getVoltage(Framework::VolData &data) override;

        AC_RET getCurrent(Framework::CurData &data) override;

        AC_RET getElectricQuantity(Framework::ElectricQuantityData &data) override;

        AC_RET updateVoltage() override;

        AC_RET updateCurrent() override;

    private:
        Driver::AdConverterChannel *_vol_adc = nullptr;
        Driver::AdConverterChannel *_cur_adc = nullptr;
    };
}

#endif //BUILD_IN_BATTERY_H_
