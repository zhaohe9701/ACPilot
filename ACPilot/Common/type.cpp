/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-08-07 22:08:18
 * @LastEditors: zhaohe
 * @LastEditTime: 2023-04-09 23:30:07
 * @FilePath: \ZH_FLIGHT\Sys\Type\type.cpp
 * Copyright (C) 2022 zhaohe. All rights reserved.
 */
#include <string.h>
#include <cstdio>
#include <cstdlib>
#include "type.h"

void Vec3::Set(float xi, float yi, float zi)
{
    x = xi;
    y = yi;
    z = zi;
}


void Vec3::Clear()
{
    x = 0;
    y = 0;
    z = 0;
}

void Vec4::Set(float xi, float yi, float zi, float wi)
{
    x = xi;
    y = yi;
    z = zi;
    w = wi;
}

void Vec4::Clear()
{
    x = 0;
    y = 0;
    z = 0;
    w = 0;
}

AC_RET Type::transTypeToStr(char *type_buf, AC_DATA_TYPE type)
{
    switch (type)
    {
        case AC_UINT8:
            strncpy(type_buf, "uint8", TYPE_BUF_LEN - 1);
            break;
        case AC_UINT16:
            strncpy(type_buf, "uint16", TYPE_BUF_LEN - 1);
            break;
        case AC_UINT32:
            strncpy(type_buf, "uint32", TYPE_BUF_LEN - 1);
            break;
        case AC_INT8:
            strncpy(type_buf, "int8", TYPE_BUF_LEN - 1);
            break;
        case AC_INT16:
            strncpy(type_buf, "int16", TYPE_BUF_LEN - 1);
            break;
        case AC_INT32:
            strncpy(type_buf, "int32", TYPE_BUF_LEN - 1);
            break;
        case AC_FLOAT:
            strncpy(type_buf, "float", TYPE_BUF_LEN - 1);
            break;
        case AC_DOUBLE:
            strncpy(type_buf, "double", TYPE_BUF_LEN - 1);
            break;
        case AC_STRING:
            strncpy(type_buf, "string", TYPE_BUF_LEN - 1);
            break;
        case AC_SWITCH:
            strncpy(type_buf, "switch", TYPE_BUF_LEN - 1);
            break;
        case AC_ENUM:
            strncpy(type_buf, "enum", TYPE_BUF_LEN - 1);
            break;
        default:
            return AC_ERROR;
    }
    return AC_OK;
}

AC_RET Type::transDataToStr(char *data_buf, void *data, uint16_t size, AC_DATA_TYPE type)
{
    if (nullptr == data_buf)
    {
        return AC_ERROR;
    }
    if (nullptr == data)
    {
        snprintf(data_buf, size, "loss");
        return AC_OK;
    }

    switch (type)
    {
        case AC_UINT8:
            snprintf(data_buf, size, "%u", (*(uint8_t *)data));
            break;
        case AC_UINT16:
            snprintf(data_buf, size, "%u", (*(uint16_t *) data));
            break;
        case AC_UINT32:
            snprintf(data_buf, size, "%lu", (*(uint32_t *) data));
            break;
        case AC_INT8:
            snprintf(data_buf, size, "%d", (*(int8_t *) data));
            break;
        case AC_INT16:
            snprintf(data_buf, size, "%d", (*(int16_t *) data));
            break;
        case AC_INT32:
            snprintf(data_buf, size, "%ld", (*(int32_t *) data));
            break;
        case AC_FLOAT:
            snprintf(data_buf, size, "%f", (*(float *) data));
            break;
        case AC_DOUBLE:
            snprintf(data_buf, size, "%f", (*(double *)data));
            break;
        case AC_SWITCH:
            snprintf(data_buf, size, "0x%x", (*(uint8_t *) data));
            break;
        case AC_STRING:
            snprintf(data_buf, size, "%s", (char*)data);
            break;
        case AC_ENUM:
            snprintf(data_buf, size, "%u", (*(uint16_t *) data));
            break;
        default:
            return AC_ERROR;
    }
    return AC_OK;
}

uint16_t Type::transStrToType(char *type_buf, AC_DATA_TYPE &type)
{
    if (0 == strncmp("uint8", type_buf, TYPE_BUF_LEN))
    {
        type = AC_UINT8;
        return sizeof(uint8_t);
    } else if (0 == strncmp("uint16", type_buf, TYPE_BUF_LEN))
    {
        type = AC_UINT16;
        return sizeof(uint16_t);
    } else if (0 == strncmp("uint32", type_buf, TYPE_BUF_LEN))
    {
        type = AC_UINT32;
        return sizeof(uint32_t);
    } else if (0 == strncmp("int8", type_buf, TYPE_BUF_LEN))
    {
        type = AC_INT8;
        return sizeof(int8_t);
    } else if (0 == strncmp("int16", type_buf, TYPE_BUF_LEN))
    {
        type = AC_INT16;
        return sizeof(int16_t);
    } else if (0 == strncmp("int32", type_buf, TYPE_BUF_LEN))
    {
        type = AC_INT32;
        return sizeof(int32_t);
    } else if (0 == strncmp("float", type_buf, TYPE_BUF_LEN))
    {
        type = AC_FLOAT;
        return sizeof(float);
    } else if (0 == strncmp("double", type_buf, TYPE_BUF_LEN))
    {
        type = AC_DOUBLE;
        return sizeof(double);
    } else if (0 == strncmp("string", type_buf, 6))
    {
        type = AC_STRING;
        return strtoul(type_buf + 8, nullptr, 10);
    } else if (0 == strncmp("switch", type_buf, TYPE_BUF_LEN))
    {
        type = AC_SWITCH;
        return sizeof(uint8_t);
    } else if (0 == strncmp("enum", type_buf, TYPE_BUF_LEN))
    {
        type = AC_ENUM;
        return sizeof(uint16_t);
    }
    else
    {
        return 0;
    }
}

AC_RET Type::transStrToData(char *data_buf, void *data, AC_DATA_TYPE type)
{
    char **end = nullptr;
    switch (type)
    {
        case AC_UINT8:
            *((uint8_t *) data) = strtoul(data_buf, end, 10);
            break;
        case AC_UINT16:
            *((uint16_t *) data) = strtoul(data_buf, end, 10);
            break;
        case AC_UINT32:
            *((uint32_t *) data) = strtoul(data_buf, end, 10);
            break;
        case AC_INT8:
            *((int8_t *) data) = strtol(data_buf, end, 10);
            break;
        case AC_INT16:
            *((int16_t *) data) = strtol(data_buf, end, 10);
            break;
        case AC_INT32:
            *((int32_t *) data) = strtol(data_buf, end, 10);
            break;
        case AC_FLOAT:
            *((float *) data) = strtof(data_buf, end);
            break;
        case AC_DOUBLE:
            *((double *) data) = strtod(data_buf, nullptr);
            break;
        case AC_SWITCH:
            *((uint8_t *) data) = strtoul(data_buf + 2, nullptr, 16);
            break;
        case AC_STRING:
            sscanf(data_buf, "%s", (char*)data);
            break;
        case AC_ENUM:
            *((uint16_t *) data) = strtoul(data_buf, end, 10);
            break;
        default:
            return AC_ERROR;
    }
    return AC_OK;
}

uint16_t Type::getAlignSize(uint16_t raw)
{
    return (raw + sizeof(int) - 1) &~ (sizeof(int) - 1);
}

