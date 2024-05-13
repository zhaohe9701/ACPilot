//
// Created by zhaohe on 2023/12/5.
//
#include "AdConverter/ad_converter_driver.h"
#include "default_debug.h"

AdConverterUnitHandle::AdConverterUnitHandle()
{
    memset(&cfg, 0, sizeof(adc_oneshot_unit_init_cfg_t));
}

AdConverterChannelHandle::AdConverterChannelHandle()
{
    memset(&cfg, 0, sizeof(adc_oneshot_chan_cfg_t));
}

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
        BASE_ERROR("adc new unit error");
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
        BASE_ERROR("adc config channel error");
        return AC_ERROR;
    }
    adc_cali_curve_fitting_config_t cali_config;
    memset(&cali_config, 0, sizeof(adc_cali_curve_fitting_config_t));
    cali_config.unit_id = _unit->handle->cfg.unit_id;
    cali_config.chan = _handle->channel;
    cali_config.atten = _handle->cfg.atten;
    cali_config.bitwidth = _handle->cfg.bitwidth;
    if (ESP_OK != adc_cali_create_scheme_curve_fitting(&cali_config, &_handle->cali))
    {
        BASE_ERROR("adc cali init error");
        return AC_ERROR;
    }
    return AC_OK;
}

AC_RET AdConverterChannel::read(int32_t &value)
{
    int raw = 0;
    if (ESP_OK != adc_oneshot_read(_unit->handle->handle, _handle->channel, &raw))
    {
        BASE_ERROR("adc read error");
        return AC_ERROR;
    }
    if (ESP_OK != adc_cali_raw_to_voltage(_handle->cali, raw, (int *) &value))
    {
        BASE_ERROR("adc cali error");
        return AC_ERROR;
    }
    return AC_OK;
}
