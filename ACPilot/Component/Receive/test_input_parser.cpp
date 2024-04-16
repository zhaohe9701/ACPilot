//
// Created by zhaohe on 2024/4/15.
//

#include "test_input_parser.h"
#include "Notify/notify.h"

AC_RET TestInputParser::parse(ComMessage &message, bool &free_message)
{
    if (0 == strcmp((const char *) &message.buf[2], "unlock"))
    {
        printf("unlock\n");
        Notify::notify(UNLOCK_COMMAND_EVENT);
    } else if (0 == strcmp((const char *) &message.buf[2], "lock"))
    {
        printf("lock\n");
        Notify::notify(LOCK_COMMAND_EVENT);
    } else if (0 == strcmp((const char *) &message.buf[2], "cali"))
    {
        printf("cali\n");
        Notify::notify(CALI_COMMAND_EVENT);
    } else if (0 == strcmp((const char *) &message.buf[2], "manual"))
    {
        printf("manual\n");
        Notify::notify(MANUAL_COMMAND_EVENT);
    } else if (0 == strcmp((const char *) &message.buf[2], "height"))
    {
        printf("height\n");
        Notify::notify(HEIGHT_COMMAND_EVENT);
    } else if (0 == strcmp((const char *) &message.buf[2], "cali finish"))
    {
        printf("cali finish\n");
        Notify::notify(CALI_FINISH_EVENT);
    } else if (0 == strcmp((const char *) &message.buf[2], "rock back"))
    {
        printf("rock back\n");
        Notify::notify(ROCK_BACK_EVENT);
    } else if (0 == strcmp((const char *) &message.buf[2], "move rock"))
    {
        printf("move rock\n");
        Notify::notify(MOVE_ROCK_EVENT);
    } else
    {
        printf("unknown event command\n");
    }
    return AC_OK;
}

bool TestInputParser::match(ComMessage &message)
{
    if ('%' == message.buf[0])
    {
        return true;
    }
    return false;
}
