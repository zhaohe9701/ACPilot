//
// Created by zhaohe on 2023/9/2.
//

#ifndef MEMORY_POOL_H_
#define MEMORY_POOL_H_
#include "type.h"
#include "Queue/ac_queue.h"
#include "List/ac_list.h"

#define MAX_BUF_NUM 32
#define BUF_POOL_NAME_LEN PARAM_NAME_LEN
class BufPool
{
public:
    BufPool(const char *name, uint32_t buf_len, uint32_t buf_num, bool allowed_dynamic);
    bool match(const char *name);
    uint8_t *alloc();
    void free(uint8_t *buf);
private:
    bool _allowed_dynamic = false;
    uint32_t _free_top = 0;
    uint32_t _buf_len = 0;
    uint32_t _buf_num = 0;
    AcQueue<uint8_t*> *_queue = nullptr;
    char _name[BUF_POOL_NAME_LEN] = {0};
};

class BufPoolManager
{
public:
    static BufPool *find(const char* name);
    static void add(BufPool * buf_pool);
private:
    static List<BufPool*> _list;
};
#endif //MEMORY_POOL_H_
