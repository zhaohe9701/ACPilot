//
// Created by zhaohe on 2024/7/24.
//
#include "default_debug.h"

using namespace Common;

namespace Common
{
    int (*acPrintf)(const char *__restrict, ...) = printf;
}
