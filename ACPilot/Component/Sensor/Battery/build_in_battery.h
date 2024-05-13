//
// Created by zhaohe on 2024/5/11.
//

#ifndef BUILD_IN_BATTERY_H_
#define BUILD_IN_BATTERY_H_


#include "Device/Physical/Battery/battery.h"
#include "AdConverter/ad_converter_driver.h"

class BuildInBattery : public Battery
{
public:
    explicit BuildInBattery(AdConverterChannel *vol_adc, AdConverterChannel *cur_adc = nullptr);

    AC_RET getVoltage(VolData &data) override;

    AC_RET getCurrent(CurData &data) override;

    AC_RET getElectricQuantity(ElectricQuantityData &data) override;

    AC_RET updateVoltage() override;

    AC_RET updateCurrent() override;

private:
    AdConverterChannel *_vol_adc = nullptr;
    AdConverterChannel *_cur_adc = nullptr;
};


#endif //BUILD_IN_BATTERY_H_
