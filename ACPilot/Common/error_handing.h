//
// Created by zhaohe on 2024/1/14.
//

#ifndef ERROR_HANDING_H_
#define ERROR_HANDING_H_

#include "type.h"

#define RETURN_CHECK(ret) {if (AC_OK != ret) {goto error;}}
#define RETURN_CHECK_WITH_LOG(ret, log) {if (AC_OK != ret) {log; goto error;}}

#define NULL_CHECK(ptr) {if (nullptr == (ptr)) {goto error;}}
#define NULL_CHECK_WITH_LOG(ptr, log) {if (nullptr == ptr) {log; goto error;}}

#endif //ERROR_HANDING_H_
