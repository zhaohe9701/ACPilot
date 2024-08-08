//
// Created by zhaohe on 2024/5/12.
//

#include "battery_monitor.h"

Utils::Mailbox<Service::LightMessage> *BatteryMonitor::light_mailbox = nullptr;
Framework::Voltmeter *BatteryMonitor::voltmeter = nullptr;

AC_RET BatteryMonitor::init()
{
    light_mailbox = Utils::Mailbox<Service::LightMessage>::find("light");
    voltmeter = Framework::VirtualDevice::find<Framework::Voltmeter>("voltmeter", Framework::VOLTMETER_DEV);
    if (light_mailbox == nullptr || voltmeter == nullptr)
    {
        BASE_ERROR("BatteryMonitor init failed");
        return AC_ERROR;
    }
    return AC_OK;
}

AC_RET BatteryMonitor::step()
{
    Framework::VolData vol_data;
    voltmeter->read(vol_data);
    if (vol_data.x < 3.5 && vol_data.x > 1.5)
    {
        Service::LightMessage msg;
        msg.id = 0x01;
        msg.mode = Service::LIGHT_FAST_FLASHING;
        light_mailbox->push(&msg);
    }
    return AC_OK;
}
