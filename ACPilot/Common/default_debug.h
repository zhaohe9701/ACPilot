//
// Created by zhaohe on 2024/1/14.
//

#ifndef DEFAULT_DEBUG_H_
#define DEFAULT_DEBUG_H_

#include "config.h"

#ifdef BASE_DEBUG
#define BASE_INFO(fmt, args...) printf("\033[1;32mINFO (%s:%d):" fmt "\033[0m\n", __func__, __LINE__, ## args)
#define BASE_WARNING(fmt, args...) printf("\033[1;33mWARN (%s:%d):" fmt "\033[0m\n", __func__, __LINE__, ## args)
#define BASE_ERROR(fmt, args...) printf("\033[1;31mERROR(%s:%d):" fmt "\033[0m\n", __func__, __LINE__, ## args)
#else
#define BASE_INFO(fmt, args...)
#define BASE_WARNING(fmt, args...)
#define BASE_ERROR(fmt, args...)
#endif

#endif //DEFAULT_DEBUG_H_
