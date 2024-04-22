//
// Created by zhaohe on 2024/4/19.
//

#include "PoseCalculating/pose_calculating.h"
#include "SensorCalibrate/sensor_calibrate.h"
#include "Notify/notify.h"

/******************对外暴露接口*****************/
extern "C" void taskInit();
/*********************************************/

void taskInit()
{
    registerPoseCalculatingTask();
    registerSensorCalibrateTask();
    Notify::notify(INIT_FINISH_EVENT);
}