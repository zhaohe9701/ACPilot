//
// Created by zhaohe on 2023/5/21.
//

#include "sys.h"
#include "Board/board.h"
#include "Protocol/Crsf/crsf.h"
#include "Memory/ac_memory.h"
#include "Debug/ac_debug.h"
#include "Json/ac_json.h"
#include "DataModule/data_module_framework.h"
#include "MemoryPool/memory_pool_framework.h"
#include "Command/DataModule/data_module_command.h"
#include "Command/command_parser.h"
#include "Command/Memory/memory_command.h"
#include "Command/Mailbox/mailbox_command.h"
#include "Command/Thread/thread_command.h"

/******************对外暴露接口*****************/
extern "C" void sysInit();

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
    DataModuleFramework::init();
    BASE_INFO("DATA MODULE SERVICE INIT");
}

void initService()
{
    MessageTransmitServer::init();
    MessageReceiveServer::init();
    CommandServer::init();
}

void startService()
{
    MessageTransmitServer::start();
    MessageReceiveServer::start();
    CommandServer::start();
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

    BASE_INFO("MAILBOX SERVICE INSTANCE ADD");
}
void createComponent()
{
    /*创建Crsf消息接收实例*/
    new CrsfParser();
    /*创建cli消息接收实例*/
    new CommandParser();

    new DataModuleCommand();

    new MemoryCommand();

    new MailboxCommand();

    new ThreadCommand();

    BASE_INFO("MESSAGE PARSER INSTANCE CREATE");
}

void sysInit()
{
    printf("Memory size:now:%lu min:%lu\n", Memory::getFreeHeapSize(), Memory::getMinimumFreeHeapSize());

    initFramework();

    addFrameworkInstance();

    initService();

    createComponent();

    startService();
}
