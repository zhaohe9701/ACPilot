//
// Created by zhaohe on 2024/4/11.
//

#ifndef EVENT_LIST_H_
#define EVENT_LIST_H_

enum Event
{
    ENTER_INIT_EVENT,               // 进入初始化状态
    ENTER_LOCK_EVENT,               // 进入锁定状态
    ENTER_MANUAL_EVENT,             // 进入手动模式
    ENTER_HEIGHT_EVENT,             // 进入定高模式
    ENTER_CALI_EVENT,               // 进入校准状态

    LEAVE_INIT_EVENT,               // 离开初始化状态
    LEAVE_LOCK_EVENT,               // 离开锁定状态
    LEAVE_MANUAL_EVENT,             // 离开手动模式
    LEAVE_HEIGHT_EVENT,             // 离开定高模式
    LEAVE_CALI_EVENT,               // 离开校准状态

    UNLOCK_COMMAND_EVENT,           // 解锁命令
    LOCK_COMMAND_EVENT,             // 锁定命令
    CALI_COMMAND_EVENT,             // 简单校准命令
    MANUAL_COMMAND_EVENT,           // 手动命令
    HEIGHT_COMMAND_EVENT,           // 定高命令

    INIT_FINISH_EVENT,              // 初始化完成
    CALI_FINISH_EVENT,              // 校准完成
    ROCK_BACK_EVENT,                // 摇杆回中
    MOVE_ROCK_EVENT,                // 摇杆移动
    WIFI_CONNECT_SUCCESS_EVENT,     // WIFI连接成功

    EVENT_NUM,
};

#endif //EVENT_LIST_H_
