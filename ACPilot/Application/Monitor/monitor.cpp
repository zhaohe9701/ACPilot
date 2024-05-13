//
// Created by zhaohe on 2024/5/12.
//

#include "monitor.h"
#include "Thread/ac_thread.h"
#include "config.h"
#include "Monitor/Battery/battery_monitor.h"

static AcThread *task = nullptr;


void monitorTask(void *param)
{
    for(;;)
    {
        BatteryMonitor::step();
        tickSleep(1000);
    }
}

void registerMonitorTask()
{
    task = new AcThread("monitor", MONITOR_TASK_STACK_SIZE, MONITOR_TASK_PRIO, MONITOR_TASK_CORE);

    BatteryMonitor::init();

    task->run(monitorTask, nullptr);
}