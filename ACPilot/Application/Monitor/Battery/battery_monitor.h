//
// Created by zhaohe on 2024/5/12.
//

#ifndef BATTERY_MONITOR_H_
#define BATTERY_MONITOR_H_

#include "type.h"
#include "Light/light_server.h"
#include "Device/Virtual/Voltmeter/voltmeter.h"

class BatteryMonitor
{
public:
    static AC_RET init();

    static AC_RET step();

private:
    static Mailbox<LightMessage> *light_mailbox;
    static Voltmeter *voltmeter;
};

#endif //BATTERY_MONITOR_H_
