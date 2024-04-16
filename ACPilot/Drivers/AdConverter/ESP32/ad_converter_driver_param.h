//
// Created by zhaohe on 2023/12/5.
//

#ifndef AD_CONVERTER_DRIVER_PARAM_H_
#define AD_CONVERTER_DRIVER_PARAM_H_

#include "esp_adc/adc_oneshot.h"
#include "Gpio/gpio_driver.h"
#include <string.h>

struct AdConverterUnitHandle
{
    AdConverterUnitHandle();

    adc_oneshot_unit_handle_t handle = nullptr;
    adc_oneshot_unit_init_cfg_t cfg{};
};

AdConverterUnitHandle::AdConverterUnitHandle()
{
    memset(&cfg, 0, sizeof(adc_oneshot_unit_init_cfg_t));
}

struct AdConverterChannelHandle
{
    AdConverterChannelHandle();

    adc_oneshot_chan_cfg_t cfg{};
    adc_cali_handle_t cali = nullptr;
    adc_channel_t channel = ADC_CHANNEL_0;
};

AdConverterChannelHandle::AdConverterChannelHandle()
{
    memset(&cfg, 0, sizeof(adc_oneshot_chan_cfg_t));
}

#endif //AD_CONVERTER_DRIVER_PARAM_H_
