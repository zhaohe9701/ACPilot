//
// Created by zhaohe on 2024/5/12.
//

#include "battery_monitor.h"

Mailbox<LightMessage> *BatteryMonitor::light_mailbox = nullptr;
Voltmeter *BatteryMonitor::voltmeter = nullptr;

AC_RET BatteryMonitor::init()
{
    light_mailbox = Mailbox<LightMessage>::find("light");
    voltmeter = VirtualDevice::find<Voltmeter>("voltmeter", VOLTMETER_DEV);
    if (light_mailbox == nullptr || voltmeter == nullptr)
    {
        BASE_ERROR("BatteryMonitor init failed");
        return AC_ERROR;
    }
    return AC_OK;
}

AC_RET BatteryMonitor::step()
{
    VolData vol_data;
    voltmeter->read(vol_data);
    if (vol_data.x < 3.5)
    {
        LightMessage msg;
        msg.id = 0x01;
        msg.mode = LIGHT_FAST_FLASHING;
        light_mailbox->push(&msg);
    }
    return AC_OK;
}
