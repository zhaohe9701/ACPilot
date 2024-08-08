//
// Created by zhaohe on 2024/4/15.
//

#ifndef TEST_INPUT_PARSER_H_
#define TEST_INPUT_PARSER_H_

#include "Receive/receive_parser.h"

namespace Component
{
    class TestInputParser : public Service::MessageReceiveParser
    {
    public:
        AC_RET parse(ComMessage &message, bool &free_message) override;

        bool match(ComMessage &message) override;
    };
}

#endif //TEST_INPUT_PARSER_H_
