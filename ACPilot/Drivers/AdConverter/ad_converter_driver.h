//
// Created by zhaohe on 2023/12/5.
//

#ifndef AD_CONVERTER_DRIVER_H_
#define AD_CONVERTER_DRIVER_H_

#ifdef C_STM32
#include "STM32/ad_converter.h"
#endif

#ifdef C_ESP32
#include "ESP32/ad_converter_driver_param.h"
#endif

#include "type.h"

class AdConverterUnit
{
public:
    explicit AdConverterUnit(AdConverterUnitHandle *handle);
    AC_RET init();
    AdConverterUnitHandle *handle = nullptr;
private:

};

class AdConverterChannel
{
public:
    explicit AdConverterChannel(AdConverterUnit *unit, AdConverterChannelHandle *handle);
    AC_RET init();
    AC_RET read(int32_t &value);
private:
    AdConverterChannelHandle *_handle = nullptr;
    AdConverterUnit *_unit = nullptr;
};
#endif //AD_CONVERTER_DRIVER_H_
