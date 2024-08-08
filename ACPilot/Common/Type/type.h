/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-08-07 22:03:11
 * @LastEditors: zhaohe
 * @LastEditTime: 2023-04-10 23:41:09
 * @FilePath: \ZH_FLIGHT\Sys\Type\type.h
 * Copyright (C) 2022 zhaohe. All rights reserved.
 */
#ifndef TYPE_H_
#define TYPE_H_

#include <stdint.h>
#include <stdio.h>


enum AC_RET
{
    AC_OK = 0,
    AC_ERROR = 1,
    AC_WARN = 2,
    AC_NOT_SUPPORT = 3
};

enum AC_DATA_TYPE
{
    AC_UNKNOWN = 0,
    AC_ROOT,
    AC_STRUCT,
    AC_ARRAY,
    AC_ENUM,
    AC_UINT8,
    AC_UINT16,
    AC_UINT32,
    AC_INT8,
    AC_INT16,
    AC_INT32,
    AC_FLOAT,
    AC_DOUBLE,
    AC_STRING,
    AC_NULL,
    AC_SWITCH,
    AC_ACTION,
    AC_DATA,
};

enum AC_RELATION
{
    AC_AND,
    AC_OR
};

#define PARAM_NAME_LEN  16
#define TYPE_BUF_LEN 16
#define DATA_BUF_LEN 64

namespace Common
{
    class Vec1
    {
    public:
        Vec1() = default;

        explicit Vec1(float xi);

        float x = 0.0f;

        void set(float xi);

        void clear();
    };

    class Vec2
    {
    public:
        Vec2() = default;

        Vec2(float xi, float yi);

        float x = 0.0f;
        float y = 0.0f;

        void set(float xi, float yi);

        void clear();
    };

    class Vec3
    {
    public:
        Vec3() = default;

        Vec3(float xi, float yi, float zi);

        float x = 0.0f;
        float y = 0.0f;
        float z = 0.0f;

        void set(float xi, float yi, float zi);

        void clear();
    };

    class Euler
    {
    public:
        Euler() = default;

        Euler(float pitchi, float rolli, float yawi);

        float roll = 0.0f;
        float pitch = 0.0f;
        float yaw = 0.0f;

        void set(float pitchi, float rolli, float yawi);

        void clear();
    };

    class Vec4
    {
    public:
        Vec4() = default;

        Vec4(float wi, float xi, float yi, float zi);

        float x = 0;
        float y = 0;
        float z = 0;
        float w = 0;

        void set(float wi, float xi, float yi, float zi);

        void clear();
    };

    class Type
    {
    public:
        static AC_RET transTypeToStr(char *type_buf, AC_DATA_TYPE type);

        static AC_RET transDataToStr(char *data_buf, uint16_t len, void *data, AC_DATA_TYPE type);

        static AC_RET transStrToType(char *type_buf, AC_DATA_TYPE &type, uint16_t &size);

        static AC_RET transStrToData(char *data_buf, uint16_t len, void *data, AC_DATA_TYPE type);
    };
}
typedef uint8_t AcSwitch;

typedef void (*TaskFunction)(void *);

#endif
