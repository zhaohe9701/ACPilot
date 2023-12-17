//
// Created by zhaohe on 2023/12/10.
//
#include <stdio.h>
#include <string.h>
#include "data_module.h"

uint8_t *DataModule::_cb_addr = nullptr;
uint8_t *DataModule::_data_addr = nullptr;
uint32_t DataModule::_cb_offset = 0;
uint32_t DataModule::_data_offset = 0;
uint32_t DataModule::_dm_size = 0;

AC_RET DataModule::init(uint32_t size)
{
    _dm_size = size;
    _cb_addr = new uint8_t[size];
    if (nullptr == _cb_addr)
    {
        return AC_ERROR;
    }
    memset(_cb_addr, 0, size);
    return AC_OK;
}

uint32_t DataModule::getFreeSize()
{
    return _dm_size - _cb_offset - _data_offset;
}

void DataModule::reset()
{
    _cb_offset = 0;
    _data_offset = 0;
    _data_addr = nullptr;
}

DataNode *DataModule::alloc(const char *name, AC_DATA_TYPE type, uint16_t size)
{
    if (DataModule::_cb_offset + sizeof(DataNode) > DataModule::_dm_size)
    {
        return nullptr;
    }
    DataNode *node = (DataNode *)(DataModule::_cb_addr + DataModule::_cb_offset);
    if (nullptr != name)
    {
        memcpy(node->_name, name, PARAM_NAME_LEN - 1);
    }
    node->_type = type;
    node->_size = size;
    DataModule::_cb_offset += sizeof(DataNode);
    return node;
}

void *DataModule::alloc(uint16_t size)
{
    if (DataModule::_cb_offset + DataModule::_data_offset + size > DataModule::_dm_size)
    {
        return nullptr;
    }
    void *data = DataModule::_data_addr + DataModule::_data_offset;
    DataModule::_data_offset += size;
    return data;
}

void DataModule::syncDataAddr()
{
    _data_addr = _cb_addr + _cb_offset;
}









