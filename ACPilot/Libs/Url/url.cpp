//
// Created by zhaohe on 2024/3/28.
//

#include <string.h>
#include "url.h"

Url::Url()
{
    memset(_buf, 0, sizeof(_buf));
}

Url::Url(const char *url)
{
    memset(_buf, 0, sizeof(_buf));

    if (nullptr == url)
    {
        return;
    }

    uint16_t i = 0;
    uint16_t j = 0;


    while (url[i] != '\0')
    {
        if ('/' != url[i])
        {
            _buf[_index][j] = url[i];
            j++;
        } else
        {
            _index++;
            j = 0;
        }
        i++;
    }
    if (j > 0)
    {
        _index++;
    }
}

char *Url::get(uint16_t index)
{
    if (index >= _index)
    {
        return nullptr;
    }
    return _buf[index];
}

void Url::push(char *url)
{
    if (_index >= URL_MAX_CLASS)
    {
        return;
    }
    strncpy(_buf[_index], url, PARAM_NAME_LEN - 1);
    _index++;
}

void Url::pop()
{
    if (0 == _index)
    {
        return;
    }
    _index--;
    memset(_buf[_index], 0, PARAM_NAME_LEN);

}

void Url::toStr(char *buf, uint16_t len)
{
    if (nullptr == buf)
    {
        return;
    }
    uint16_t i = 0;
    uint16_t j = 0;
    uint16_t k = 0;
    for (i = 0; i < _index; i++)
    {
        while (_buf[i][j] != '\0')
        {
            buf[k] = _buf[i][j];
            j++;
            k++;
        }
        buf[k] = '/';
        j = 0;
        k++;
    }
    if (k > 0)
    {
        buf[k - 1] = '\0';
    }
}

