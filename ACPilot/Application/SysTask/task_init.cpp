//
// Created by zhaohe on 2024/4/19.
//

#include "Control/PoseCalculating/pose_calculating.h"
#include "SensorCalibrate/sensor_calibrate.h"
#include "Notify/notify.h"
#include "Control/control.h"
#include "Monitor/monitor.h"

/******************对外暴露接口*****************/
extern "C" void taskInit();
/*********************************************/

void taskInit()
{
    PoseCalculating::init();
    registerSensorCalibrateTask();
    registerMonitorTask();
    registerControlTask();
    BASE_INFO("TASK INIT FINISH");
}