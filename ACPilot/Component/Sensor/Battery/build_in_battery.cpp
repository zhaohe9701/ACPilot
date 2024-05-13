//
// Created by zhaohe on 2024/5/11.
//

#include "build_in_battery.h"

#define PARTIAL_VOLTAGE_RESISTANCE_RATIO 0.5f
#define CURRENT_SAMPLING_RESISTANCE 0.1f

BuildInBattery::BuildInBattery(AdConverterChannel *vol_adc, AdConverterChannel *cur_adc)
{
    if (vol_adc == nullptr)
    {
        return;
    }

    _vol_adc = vol_adc;
    _cur_adc = cur_adc;

    _ability = 1U << VOLTMETER_DEV;

    if (_cur_adc != nullptr)
    {
        _ability |= (1U << AMMETER_DEV);
    }
    strncpy(_name, "BuildInBattery", sizeof(_name));
}

AC_RET BuildInBattery::getVoltage(VolData &data)
{
    data = _vol_data;
    return AC_OK;
}

AC_RET BuildInBattery::getCurrent(CurData &data)
{
    if (_cur_adc == nullptr)
    {
        return AC_NOT_SUPPORT;
    }
    data = _cur_data;
    return AC_OK;
}

AC_RET BuildInBattery::getElectricQuantity(ElectricQuantityData &data)
{
    data = _electric_quantity_data;
    return AC_OK;
}

AC_RET BuildInBattery::updateVoltage()
{
    int32_t raw = 0;
    _vol_adc->read(raw);
    _vol_data.x = (float)raw / PARTIAL_VOLTAGE_RESISTANCE_RATIO / 1000.0f;
    return AC_OK;
}

AC_RET BuildInBattery::updateCurrent()
{
    int32_t raw = 0;
    _cur_adc->read(raw);
    _cur_data.x = (float)raw / 1000.0f / CURRENT_SAMPLING_RESISTANCE;
    return AC_OK;
}

