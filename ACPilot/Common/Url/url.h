//
// Created by zhaohe on 2024/3/28.
//

#ifndef URL_H_
#define URL_H_

#include "Type/type.h"

namespace Common
{

#define URL_MAX_CLASS 10
#define URL_MAX_LEN 128

    class Url
    {
    public:
        explicit Url(const char *url);

        Url();

        char *get(uint16_t index);

        void push(char *url);

        void pop();

        void toStr(char *buf, uint16_t len);

    private:
        char _buf[URL_MAX_CLASS][PARAM_NAME_LEN];
        uint16_t _index = 0;
    };

}

#endif //URL_H_
