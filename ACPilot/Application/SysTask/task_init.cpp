//
// Created by zhaohe on 2024/4/19.
//

#include "Control/PoseCalculating/pose_calculating.h"
#include "SensorCalibrate/sensor_calibrate.h"
#include "Notify/notify.h"
#include "Control/control.h"

/******************对外暴露接口*****************/
extern "C" void taskInit();
/*********************************************/

void taskInit()
{
    PoseCalculating::init();
    registerSensorCalibrateTask();
    registerControlTask();
}