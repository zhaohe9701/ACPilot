//
// Created by zhaohe on 2023/9/2.
//

#ifndef MEMORY_POOL_H_
#define MEMORY_POOL_H_

#include "Type/type.h"
#include "Queue/queue.h"
#include "List/ac_list.h"
#include "config.h"

namespace Utils
{

#define BUF_POOL_NAME_LEN PARAM_NAME_LEN

    class MemoryPool
    {
    public:
        MemoryPool(const char *name, uint32_t size, uint32_t num, bool allowed_dynamic);

        bool match(const char *name);

        char *getName();

        uint32_t getSize();

        uint32_t getUsedNum();

        uint32_t getFreeNum();

        uint32_t getNum();

        bool isDynamic();

        uint8_t *alloc();

        void free(uint8_t *buf);

        static MemoryPool *find(const char *name);

        static void add(MemoryPool *buf_pool);

        static MemoryPool *getGeneral(uint32_t size);

        static Common::List<MemoryPool *> *getList();

        static MemoryPool **getGeneralPoolList();

    private:
        bool _allowed_dynamic = false;
        uint8_t *_head = nullptr;
        uint32_t _size = 0;
        uint32_t _num = 0;
        char _name[BUF_POOL_NAME_LEN] = {0};
        Osal::Queue<uint8_t *> *_queue = nullptr;
        static MemoryPool *_general_memory_pool[GENERAL_MEMORY_POOL_NUM];
        static Common::List<MemoryPool *> _list;
    };
}
#endif //MEMORY_POOL_H_
