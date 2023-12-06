//
// Created by zhaohe on 2023/8/10.
//

#ifndef MEMORY_H_
#define MEMORY_H_

#include <stddef.h>  // for size_t

void* operator new(size_t size);
void operator delete(void* p) noexcept;

void* operator new[](size_t size);
void operator delete[](void* p) noexcept;

#endif //MEMORY_H_
