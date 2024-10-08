//
// Created by zhaohe on 2023/9/2.
//

#include "memory_pool.h"
#include "string.h"

using namespace Utils;

Common::List<MemoryPool *> MemoryPool::_list;
MemoryPool *MemoryPool::_general_memory_pool[GENERAL_MEMORY_POOL_NUM] = {nullptr};


MemoryPool::MemoryPool(const char *name, uint32_t size, uint32_t num, bool allowed_dynamic)
{
    _queue = new Osal::Queue<uint8_t *>(num);
    strncpy(_name, name, BUF_POOL_NAME_LEN);
    uint8_t *buf = nullptr;

    _size = size;
    _num = num;
    _allowed_dynamic = allowed_dynamic;
    _head = new uint8_t[_size * _num];

    memset(_head, 0, _size * _num);

    for (uint32_t i = 0; i < _num; ++i)
    {
        buf = _head + i * _size;
        _queue->push(&buf, AC_IMMEDIATELY);
    }
    add(this);
}

bool MemoryPool::match(const char *name)
{
    if (0 == strncmp(_name, name, BUF_POOL_NAME_LEN))
    {
        return true;
    }
    return false;
}

char *MemoryPool::getName()
{
    return _name;
}

uint8_t *MemoryPool::alloc()
{
    uint8_t *buf = nullptr;

    if (AC_OK == _queue->pop(&buf, AC_IMMEDIATELY))
    {
        return buf;
    } else
    {
        if (_allowed_dynamic)
        {
            return new uint8_t[_size];
        }
        return nullptr;
    }
}

void MemoryPool::free(uint8_t *buf)
{
    if (nullptr == buf)
    {
        return;
    }
    if (buf >= _head && buf < _head + _size * _num)
    {
        memset(buf, 0, _size);
        _queue->push(&buf, AC_IMMEDIATELY);
        return;
    }
    delete buf;
}

uint32_t MemoryPool::getUsedNum()
{
    return _num - _queue->getNum();
}

uint32_t MemoryPool::getFreeNum()
{
    return _queue->getNum();
}

bool MemoryPool::isDynamic()
{
    return _allowed_dynamic;
}

uint32_t MemoryPool::getNum()
{
    return _num;
}

uint32_t MemoryPool::getSize()
{
    return _size;
}

void MemoryPool::add(MemoryPool *buf_pool)
{
    _list.pushBack(buf_pool);
}

MemoryPool *MemoryPool::find(const char *name)
{
    for (Common::ListNode<MemoryPool *> *it = _list.begin(); it != _list.end(); it = it->getNext())
    {
        if ((**it)->match(name))
        {
            return (**it);
        }
    }
    return nullptr;
}

MemoryPool *MemoryPool::getGeneral(uint32_t size)
{
    for (uint16_t i = 0; i < GENERAL_MEMORY_POOL_NUM; ++i)
    {
        if (_general_memory_pool[i]->getSize() >= size)
        {
            return _general_memory_pool[i];
        }
    }

    return nullptr;
}

Common::List<MemoryPool *> *MemoryPool::getList()
{
    return &_list;
}

MemoryPool **MemoryPool::getGeneralPoolList()
{
    return _general_memory_pool;
}




