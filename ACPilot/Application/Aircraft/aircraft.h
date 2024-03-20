/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-12-22 23:42:17
 * @LastEditors: zhaohe
 * @LastEditTime: 2023-04-09 23:16:01
 * @FilePath: \ZH_FLIGHT\Sys\Aircraft\aircraft.h
 * Copyright (C) 2022 zhaohe. All rights reserved.
 */
#ifndef AIRCRAFT_H_
#define AIRCRAFT_H_

#include "type.h"
#include "Sensor/sensor.h"
#include "Actuator/Motor/motor.h"
#include "AircraftState/aircraft_state.h"
#include "config.h"
#include "Control/AttitudeControl/default_attitude_controller.h"
#include "AHRS/ahrs.h"
#include "StateMachine/state_machine.h"
#include "Mail/mailbox.h"
#include "Actuator/actuator_data.h"
#include "Remote/remote_data.h"
#include "Debug/print.h"
#include "Light/light.h"
#include "Remote/remote_channel.h"

class Aircraft
{
private:
    /*传感器*/
    Sensor *_sensor = nullptr;
    /*电机*/
    Motor *_motors = nullptr;

    Mailbox<ImuData> _imu_data_manager;
    Mailbox<BaroData> _baro_data_manager;
    Mailbox<ActualAttitudeState> _actual_state_manager;
    Mailbox<ExpectAttitudeState> _expect_state_manager;
    Mailbox<ActuatorData> _expect_actuator_manager;
    /*姿态控制器*/
    AttitudeController *_attitude_controller = nullptr;
    /*姿态解算器*/
    AttitudeSolver *_attitude_solver = nullptr;
    /*当前动作*/

    RemoteChannelMapper *_channel_maper = nullptr;

    RemoteData _current_remote;
public:
    Mailbox<RemoteData> remote_data_manager;
    Aircraft();
    AC_RET init();
    AC_RET getRemoteCommand();
    AC_RET getAccAndGyro();
    AC_RET getAltitude();
    AC_RET getMagnetic();
    AC_RET updateAttitude();
    AC_RET controlAttitude();
    AC_RET controlAltitude();
    AC_RET controlMotor();
    AC_RET test();
    ~Aircraft();

};



#endif
