//
// Created by zhaohe on 2023/8/8.
//

#ifndef AC_ARRAY_H_
#define AC_ARRAY_H_

#include <stdint.h>
#include <string.h>
#include "Memory/ac_memory.h"

template <class T>
class Array
{
private:
    T *_data = nullptr;
public:
    Array();
    Array(uint16_t len);
    void pushBack(T &data);
    uint16_t size();
    T& operator[](uint16_t i);
    ~Array();
private:
    uint16_t _size = 0;
};

template <class T>
Array<T>::Array(uint16_t len)
{
    _data = new T[len]();
}

template <class T>
void Array<T>::pushBack(T &data)
{
    T *temp = new T[++_size]();
    memcpy(temp, _data, _size - 1);
    temp[_size] = data;
    delete[] _data;
    _data = temp;
}

template <class T>
T& Array<T>::operator[](uint16_t i)
{
    return _data[i];
}

template <class T>
uint16_t Array<T>::size()
{
    return _size;
}

template <class T>
Array<T>::~Array()
{
    delete[] _data;
}

#endif //AC_ARRAY_H_
