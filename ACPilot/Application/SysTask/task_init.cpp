//
// Created by zhaohe on 2023/11/5.
//
#include "Thread/ac_thread.h"

/******************对外暴露接口*****************/
extern "C" void taskInit();
/*********************************************/

extern void testTask(void *argument);

#include "Thread/ac_thread.h"

void taskInit()
{
    /* 创建任务实例 */
    AcThread *test_task = new AcThread("test", 1024, 12);
    /* 启动任务 */
    test_task->run(testTask, nullptr);
}