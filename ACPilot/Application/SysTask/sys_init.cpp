//
// Created by zhaohe on 2023/5/21.
//

#include "Board/board_esp32_mini.h"
#include "Protocol/Crsf/crsf.h"
#include "Memory/ac_memory.h"
#include "Debug/ac_debug.h"
#include "Json/ac_json.h"
#include "DataModule/data_module.h"
#include "MemoryPool/memory_pool_manager.h"
#include "Command/DataModule/data_module_command.h"
#include "Command/command_parser.h"
#include "Command/Memory/memory_command.h"
#include "Command/Mailbox/mailbox_command.h"
#include "Command/Thread/thread_command.h"
#include "Transmit/transmit_server.h"
#include "Receive/receive_server.h"
#include "Command/command_server.h"
#include "StateMachine/state_machine.h"
#include "Receive/test_input_parser.h"
#include "Light/light_server.h"
#include "Light/Led/led.h"
#include "Command/Calibrate/calibrate_command.h"
#include "Command/Nvs/nvs_command.h"

/******************对外暴露接口*****************/
extern "C" void frameworkInit();
extern "C" void serviceInit();
/*********************************************/




void initFramework()
{
    /* 初始化内存池服务 */
    MemoryPoolManager::init();
    BASE_INFO("MEMORY POOL SERVICE INIT");
    /* 初始化debug服务 */
    Debug::init();
    BASE_INFO("DEBUG SERVICE INIT");
    /* 初始化数据模型服务 */
    DataModule::init();
    BASE_INFO("DATA MODULE SERVICE INIT");
}

void initService()
{
    MessageTransmitServer::init();
    MessageReceiveServer::init();
    CommandServer::init();
    StateMachine::init();
    LightServer::init();
}

void startService()
{
    MessageTransmitServer::start();
    MessageReceiveServer::start();
    CommandServer::start();
    StateMachine::start();
    LightServer::start();
}

void addFrameworkInstance()
{
    /* 创建内存资源实例 */
    new MemoryPool("json", sizeof(JsonTree), 100, true);

    BASE_INFO("MEMORY POOL SERVICE INSTANCE ADD");

    /* 创建接收信箱实例 */
    new Mailbox<ComMessage>("receive", 10);
    /* 创建发送信箱实例 */
    new Mailbox<ComMessage>("send", 10);
    /* 创建命令信箱实例 */
    new Mailbox<CommandMessage>("command", 5);
    /* 创建指令信箱实例 */
    new Mailbox<RemoteData>("remote", 1);
    /* 灯指令信箱实例 */
    new Mailbox<LightMessage>("light", 1);

    new Mailbox<CaliMessage>("cali", 1);

    BASE_INFO("MAILBOX SERVICE INSTANCE ADD");
}
void createComponent()
{
    /*创建Crsf消息接收实例*/
    new CrsfParser();
    /*创建cli消息接收实例*/
    new CommandParser();

    new TestInputParser();

    new DataModuleCommand();

    new MemoryCommand();

    new MailboxCommand();

    new ThreadCommand();

    new CalibrateCommand();

    new NvsCommand();

    new Led(Board::led_pin, GPIO_RESET, LIGHT_BREATHE, 0x01);

    State *state_init       = new State("init", ENTER_INIT_EVENT, LEAVE_INIT_EVENT);
    State *state_lock       = new State("lock", ENTER_LOCK_EVENT, LEAVE_LOCK_EVENT);
    State *state_unlocking  = new State("unlocking", ENTER_UNLOCKING_EVENT, LEAVE_UNLOCKING_EVENT);
    State *state_ready      = new State("ready", ENTER_READY_EVENT, LEAVE_READY_EVENT);
    State *state_manual     = new State("manual", ENTER_MANUAL_EVENT, LEAVE_MANUAL_EVENT);
    State *state_height     = new State("height", ENTER_HEIGHT_EVENT, LEAVE_HEIGHT_EVENT);
    State *state_calibrate  = new State("calibrate", ENTER_CALIBRATE_EVENT, LEAVE_CALIBRATE_EVENT);

    //                                          状态转换表
    //--------------------------------------------------------------------------------------------------------------------
    //|         |init       |lock         |unlocking        |ready         |manual         |height        |calibrate     |
    //--------------------------------------------------------------------------------------------------------------------
    //|init     | *         |INIT_FINISH  | -               | -            | -             | -            | -            |
    //--------------------------------------------------------------------------------------------------------------------
    //|lock     | -         | *           |UNLOCK_COMMAND   | -            | -             | -            | CALI_COMMAND |
    //--------------------------------------------------------------------------------------------------------------------
    //|unlocking| -         | -           | *               |ROCK_BACK     | -             | -            | -            |
    //--------------------------------------------------------------------------------------------------------------------
    //|ready    | -         |MOVE_ROCK    | -               | *            |MANUAL_COMMAND |HEIGHT_COMMAND| -            |
    //--------------------------------------------------------------------------------------------------------------------
    //|manual   | -         |LOCK_COMMAND | HEIGHT_COMMAND  | -            | *             | -            | -            |
    //--------------------------------------------------------------------------------------------------------------------
    //|height   | -         |LOCK_COMMAND | -               | -            | -             | *            | -            |
    //--------------------------------------------------------------------------------------------------------------------
    //|calibrate| -         |CALI_FINISH  | -               | -            | -             | -            | *            |
    //--------------------------------------------------------------------------------------------------------------------

    state_init->addNextState(state_lock, INIT_FINISH_EVENT);

    state_lock->addNextState(state_unlocking, UNLOCK_COMMAND_EVENT);
    state_lock->addNextState(state_calibrate, CALI_COMMAND_EVENT);

    state_unlocking->addNextState(state_ready, ROCK_BACK_EVENT);

    state_ready->addNextState(state_manual, MANUAL_COMMAND_EVENT);
    state_ready->addNextState(state_height, HEIGHT_COMMAND_EVENT);
    state_ready->addNextState(state_lock, MOVE_ROCK_EVENT);

    state_manual->addNextState(state_lock, LOCK_COMMAND_EVENT);
    state_manual->addNextState(state_height, HEIGHT_COMMAND_EVENT);

    state_height->addNextState(state_lock, LOCK_COMMAND_EVENT);

    state_calibrate->addNextState(state_lock, CALI_FINISH_EVENT);

    StateMachine::setStartState(state_init);

    BASE_INFO("MESSAGE PARSER INSTANCE CREATE");
}

void frameworkInit()
{
    initFramework();

    addFrameworkInstance();
}

void serviceInit()
{
    initService();

    createComponent();

    startService();
}