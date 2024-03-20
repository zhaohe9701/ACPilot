//
// Created by zhaohe on 2023/9/2.
//

#include "memory_pool.h"
#include "Memory/ac_memory.h"
#include "string.h"

MemoryPool::MemoryPool(const char *name, uint32_t buf_len, uint32_t buf_num, bool allowed_dynamic)
{
    _queue = new AcQueue<uint8_t *>(MAX_BUF_NUM);
    strncpy(_name, name, BUF_POOL_NAME_LEN);
    uint8_t *buf = nullptr;

    if (buf_num > MAX_BUF_NUM)
    {
        return;
    }
    _buf_len = buf_len;
    _buf_num = buf_num;
    _allowed_dynamic = allowed_dynamic;

    for (uint32_t i = 0; i < _buf_num; ++i)
    {
        buf = new uint8_t[_buf_len];
        if (nullptr == buf)
        {
            return;
        }
        memset(buf, 0, _buf_len);
        _queue->push(&buf, AC_IMMEDIATELY);
    }
    MemoryPoolManager::add(this);
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
        if (_allowed_dynamic && _buf_num < MAX_BUF_NUM)
        {
            buf = new uint8_t[_buf_len];
            if (nullptr == buf)
            {
                return nullptr;
            }
            memset(buf, 0, _buf_len);
            _queue->push(&buf, AC_IMMEDIATELY);
            _buf_num++;
        }
        return nullptr;
    }
}

void MemoryPool::free(uint8_t *buf)
{
    memset(buf, 0, _buf_len);
//    debugPrintf("free:%p\n", buf);
    _queue->push(&buf, AC_IMMEDIATELY);
}

List<MemoryPool *> MemoryPoolManager::_list;

void MemoryPoolManager::add(MemoryPool *buf_pool)
{
    _list.pushBack(buf_pool);
}

MemoryPool *MemoryPoolManager::find(const char *name)
{
    for (ListNode<MemoryPool *> *it = _list.begin(); it != _list.end(); it = it->getNext())
    {
        if ((**it)->match(name))
        {
            return (**it);
        }
    }
    return nullptr;
}
