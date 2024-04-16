//
// Created by zhaohe on 2023/12/5.
//
#include "AdConverter/ad_converter_driver.h"


AdConverterUnit::AdConverterUnit(AdConverterUnitHandle *handle)
{
    this->handle = handle;
}

AC_RET AdConverterUnit::init()
{
    if (nullptr == handle)
    {
        return AC_ERROR;
    }
    if (ESP_OK != adc_oneshot_new_unit(&handle->cfg, &handle->handle))
    {
        return AC_ERROR;
    }
    return AC_OK;
}

AdConverterChannel::AdConverterChannel(AdConverterUnit *unit, AdConverterChannelHandle *handle)
{
    _handle = handle;
    _unit = unit;
}

AC_RET AdConverterChannel::init()
{
    if (nullptr == _unit || nullptr == _handle)
    {
        return AC_ERROR;
    }
    if (ESP_OK != adc_oneshot_config_channel(_unit->handle->handle, _handle->channel, &_handle->cfg))
    {
        return AC_ERROR;
    }
    adc_cali_curve_fitting_config_t cali_config;
    memset(&cali_config, 0, sizeof(adc_cali_curve_fitting_config_t));
    cali_config.unit_id = _unit->handle->cfg.unit_id;
    cali_config.chan = _handle->channel;
    cali_config.atten = _handle->cfg.atten;
    cali_config.bitwidth = ADC_BITWIDTH_DEFAULT;
    if (ESP_OK != adc_cali_create_scheme_curve_fitting(&cali_config, &_handle->cali))
    {
        return AC_ERROR;
    }
    return AC_OK;
}

AC_RET AdConverterChannel::read(int32_t &value)
{
    int raw = 0;
    if (ESP_OK != adc_oneshot_read(_unit->handle->handle, _handle->channel, &raw))
    {
        return AC_ERROR;
    }
    if (ESP_OK != adc_cali_raw_to_voltage(_handle->cali, raw, (int *) &value))
    {
        return AC_ERROR;
    }
    return AC_OK;
}
