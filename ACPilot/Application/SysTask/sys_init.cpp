//
// Created by zhaohe on 2023/5/21.
//

#include "Board/board_esp32_mini.h"
#include "Receive/Crsf/crsf.h"
#include "Memory/memory.h"
#include "Debug/ac_debug.h"
#include "Json/json.h"
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
#include "AHRS/attitude.h"

/******************对外暴露接口*****************/
extern "C" void frameworkInit();
extern "C" void serviceInit();
extern "C" void lightControlInit();
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
    Framework::DataModule::init();
    BASE_INFO("DATA MODULE SERVICE INIT");
}

void initService()
{
    Service::MessageTransmitServer::init();
    Service::MessageReceiveServer::init();
    Service::CommandServer::init();
    Service::StateMachine::init();
    Service::LightServer::init();
}

void startService()
{
    Service::MessageTransmitServer::start();
    Service::MessageReceiveServer::start();
    Service::CommandServer::start();
    Service::StateMachine::start();
    Service::LightServer::start();
}

void addFrameworkInstance()
{
    /* 创建内存资源实例 */
    new Utils::MemoryPool("json", sizeof(Utils::JsonTree), 10, true);

    BASE_INFO("MEMORY POOL SERVICE INSTANCE ADD");

    /* 创建接收信箱实例 */
    new Utils::Mailbox<ComMessage>("receive", 10);
    /* 创建发送信箱实例 */
    new Utils::Mailbox<ComMessage>("send", 10);
    /* 创建命令信箱实例 */
    new Utils::Mailbox<CommandMessage>("command", 5);
    /* 创建指令信箱实例 */
    new Utils::Mailbox<RemoteData>("remote", 1);
    /* 灯指令信箱实例 */
    new Utils::Mailbox<Service::LightMessage>("light", 5);

    new Utils::Mailbox<Component::CaliMessage>("cali", 1);

    new Utils::Mailbox<Component::PoseData>("attitude", 1);

    BASE_INFO("MAILBOX SERVICE INSTANCE ADD");
}
void createComponent()
{
    /*创建Crsf消息接收实例*/
    new Component::CrsfParser();
    /*创建cli消息接收实例*/
    new Service::CommandParser();

    new Component::TestInputParser();

    new Component::DataModuleCommand();

    new Component::MemoryCommand();

    new Component::MailboxCommand();

    new Component::ThreadCommand();

    new Component::CalibrateCommand();

    new Component::NvsCommand();

    new Component::Led(Board::led_pin, Driver::GPIO_RESET, Service::LIGHT_KEEP_OFF, 0x01);

    Service::State *state_init       = new Service::State("init", ENTER_INIT_EVENT, LEAVE_INIT_EVENT);
    Service::State *state_lock       = new Service::State("lock", ENTER_LOCK_EVENT, LEAVE_LOCK_EVENT);
    Service::State *state_manual     = new Service::State("manual", ENTER_MANUAL_EVENT, LEAVE_MANUAL_EVENT);
    Service::State *state_height     = new Service::State("height", ENTER_HEIGHT_EVENT, LEAVE_HEIGHT_EVENT);
    Service::State *state_calibrate  = new Service::State("calibrate", ENTER_CALI_EVENT, LEAVE_CALI_EVENT);

    //                                          状态转换表
    //-----------------------------------------------------------------------------------
    //|         |init       |lock         |manual         |height        |calibrate     |
    //-----------------------------------------------------------------------------------
    //|init     | *         |INIT_FINISH  | -             | -            | CALI_COMMAND |
    //-----------------------------------------------------------------------------------
    //|lock     | -         | *           |MANUAL_EVENT   |HEIGHT_COMMAND| CALI_COMMAND |
    //-----------------------------------------------------------------------------------
    //|manual   | -         |LOCK_COMMAND | *             | -            | -            |
    //-----------------------------------------------------------------------------------
    //|height   | -         |LOCK_COMMAND | -             | *            | -            |
    //-----------------------------------------------------------------------------------
    //|calibrate| -         |CALI_FINISH  | -             | -            | *            |
    //-----------------------------------------------------------------------------------

    state_init->addNextState(state_lock, INIT_FINISH_EVENT);
    state_init->addNextState(state_calibrate, CALI_COMMAND_EVENT);

    state_lock->addNextState(state_manual, MANUAL_COMMAND_EVENT);
    state_lock->addNextState(state_height, HEIGHT_COMMAND_EVENT);
    state_lock->addNextState(state_calibrate, CALI_COMMAND_EVENT);

    state_manual->addNextState(state_lock, LOCK_COMMAND_EVENT);
    state_height->addNextState(state_lock, LOCK_COMMAND_EVENT);
    state_calibrate->addNextState(state_lock, CALI_FINISH_EVENT);

    Service::StateMachine::setStartState(state_init);

    BASE_INFO("MESSAGE PARSER INSTANCE CREATE");
}

static Utils::Mailbox<Service::LightMessage> *light_mailbox = nullptr;

static void setLight(Service::LightMode light_mode)
{
    Service::LightMessage light_msg;
    light_msg.id = 0x01;
    light_msg.mode = light_mode;
    light_mailbox->push(&light_msg, AC_IMMEDIATELY);
}

void eventHandle(void *param)
{
    Utils::NotifyToken *token = (Utils::NotifyToken *) param;
    switch (token->getEvent())
    {
        case ENTER_INIT_EVENT:
            setLight(Service::LIGHT_KEEP_OFF);
            break;
        case ENTER_LOCK_EVENT:
            setLight(Service::LIGHT_BREATHE);
            break;
        case ENTER_MANUAL_EVENT:
            setLight(Service::LIGHT_PULSE_FLASHING);
            break;
        case ENTER_HEIGHT_EVENT:
            setLight(Service::LIGHT_DOUBLE_PULSE_FLASHING);
            break;
        case ENTER_CALI_EVENT:
            setLight(Service::LIGHT_SLOW_FLASHING);
            break;
        default:
            break;
    }
}

void lightControlInit()
{
    light_mailbox = Utils::Mailbox<Service::LightMessage>::find("light");
    if (light_mailbox == nullptr)
    {
        BASE_ERROR("light mailbox not found");
        return;
    }
    Utils::Notify::sub(ENTER_INIT_EVENT, eventHandle, nullptr);
    Utils::Notify::sub(ENTER_LOCK_EVENT, eventHandle, nullptr);
    Utils::Notify::sub(ENTER_MANUAL_EVENT, eventHandle, nullptr);
    Utils::Notify::sub(ENTER_HEIGHT_EVENT, eventHandle, nullptr);
    Utils::Notify::sub(ENTER_CALI_EVENT, eventHandle, nullptr);
    BASE_INFO("LIGHT CONTROL INIT FINISH");
}




void frameworkInit()
{
    initFramework();

    addFrameworkInstance();
    BASE_INFO("FRAMEWORK INIT FINISH");
}

void serviceInit()
{
    initService();

    createComponent();

    startService();
    BASE_INFO("SERVICE INIT FINISH");
}