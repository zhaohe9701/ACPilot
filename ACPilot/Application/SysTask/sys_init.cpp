//
// Created by zhaohe on 2023/5/21.
//

#include "sys.h"
#include "Board/board.h"
#include "Protocol/Crsf/crsf.h"
#include "Memory/ac_memory.h"
#include "Debug/ac_debug.h"

/******************对外暴露接口*****************/
extern "C" void sysInit();
/*********************************************/

void sysInit()
{
    /* 创建内存资源实例 */
    new BufPool("message", MAX_MESSAGE_BUF_LEN, 30, true);
    /* 创建接收信箱实例 */
    new Mailbox<ComMessage>("receive", 10);
    /* 创建发送信箱实例 */
    new Mailbox<ComMessage>("send", 10);
    /* 创建命令信箱实例 */
    new Mailbox<CommandMessage>("command", 5);
    /* 创建指令信箱实例 */
    new Mailbox<RemoteData>("remote", 1);
    /* 创建事件信箱实例 */
    new Mailbox<Event>("event", EVENT_QUEUE_LEN);
    /*创建Crsf消息接收实例*/
    new CrsfParser();
    /*创建cli消息接收实例*/
    new CommandParser();
    /*初始化debug*/
    Debug::init();
}
