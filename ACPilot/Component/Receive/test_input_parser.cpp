//
// Created by zhaohe on 2024/4/15.
//

#include "test_input_parser.h"
#include "Notify/notify.h"

using namespace Component;

AC_RET TestInputParser::parse(ComMessage &message, bool &free_message)
{
    if (0 == strcmp((const char *) &message.buf[2], "unlock"))
    {
        printf("unlock\n");
        Utils::Notify::notify(UNLOCK_COMMAND_EVENT);
    } else if (0 == strcmp((const char *) &message.buf[2], "lock"))
    {
        printf("lock\n");
        Utils::Notify::notify(LOCK_COMMAND_EVENT);
    } else if (0 == strcmp((const char *) &message.buf[2], "cali"))
    {
        printf("cali\n");
        Utils::Notify::notify(CALI_COMMAND_EVENT);
    } else if (0 == strcmp((const char *) &message.buf[2], "manual"))
    {
        printf("manual\n");
        Utils::Notify::notify(MANUAL_COMMAND_EVENT);
    } else if (0 == strcmp((const char *) &message.buf[2], "height"))
    {
        printf("height\n");
        Utils::Notify::notify(HEIGHT_COMMAND_EVENT);
    } else if (0 == strcmp((const char *) &message.buf[2], "cali finish"))
    {
        printf("cali finish\n");
        Utils::Notify::notify(CALI_FINISH_EVENT);
    } else if (0 == strcmp((const char *) &message.buf[2], "rock back"))
    {
        printf("rock back\n");
        Utils::Notify::notify(ROCK_BACK_EVENT);
    } else if (0 == strcmp((const char *) &message.buf[2], "move rock"))
    {
        printf("move rock\n");
        Utils::Notify::notify(MOVE_ROCK_EVENT);
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
