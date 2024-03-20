//
// Created by zhaohe on 2024/1/22.
//
#include <new>
#include <string.h>
#include "data_module.h"
#include "error_handing.h"
#include "default_debug.h"

static uint16_t getArrayNum(char *key)
{
    int i = 0;
    while (key[i] != '\0')
    {
        if (key[i] == '[')
        {
            return strtol(key + i + 1, nullptr, 10);
        }
        i++;
    }
    return 0;
}

/* 打印Data Module树结构的仿函数 */
class DumpDataModule : public TreeVisit
{
public:
    DumpDataModule(char *buf, uint32_t len);

    AC_RET operator()(Tree *node, TREE_VISIT_ORDER order) override;

private:
    char *_buf = nullptr;
    uint32_t _len = 0;
};

AC_RET DumpDataModule::operator()(Tree *node, TREE_VISIT_ORDER order)
{
    DataTree *tree = static_cast<DataTree *>(node);
    int len = 0;
    if (TREE_PRE_ORDER == order)
    {
        if (nullptr != tree->getParent() && AC_ARRAY != tree->getParent()->getType())
        {
            if (0 > (len = snprintf(_buf, _len, R"("%s":)", tree->getKey())))
            {
                BASE_ERROR("snprintf error");
                return AC_ERROR;
            }
            _buf += len;
        }
        if (AC_ROOT == tree->getType() || AC_STRUCT == tree->getType())
        {
            if (0 > (len = snprintf(_buf, _len, "{")))
            {
                BASE_ERROR("snprintf error");
                return AC_ERROR;
            }
            _buf += len;
        } else if (AC_ARRAY == tree->getType())
        {
            if (0 > (len = snprintf(_buf, _len, "[")))
            {
                BASE_ERROR("snprintf error");
                return AC_ERROR;
            }
            _buf += len;
        } else
        {
            char type[TYPE_BUF_LEN] = {0};
            Type::transTypeToStr(type, tree->getType());
            if (0 > (len = snprintf(_buf, _len, R"("%s")", type)))
            {
                BASE_ERROR("snprintf error");
                return AC_ERROR;
            }
            _buf += len;
        }
    } else if (TREE_POST_ORDER == order)
    {
        if (AC_STRUCT == tree->getType() || AC_ROOT == tree->getType())
        {
            if (0 > (len = snprintf(_buf, _len, "}")))
            {
                BASE_ERROR("snprintf error");
                return AC_ERROR;
            }
            _buf += len;
        } else if (AC_ARRAY == tree->getType())
        {
            if (0 > (len = snprintf(_buf, _len, "]")))
            {
                BASE_ERROR("snprintf error");
                return AC_ERROR;
            }
            _buf += len;
        } else
        {
            _buf += len;
        }
        if (nullptr != tree->getNeighbor())
        {
            if (0 > (len = snprintf(_buf, _len, ",")))
            {
                BASE_ERROR("snprintf error");
                return AC_ERROR;
            }
            _buf += len;
        }
    }
    return AC_OK;
}

DumpDataModule::DumpDataModule(char *buf, uint32_t len)
{
    _buf = buf;
    _len = len;
}

/* 计算Data Module树(子)节点和数量的仿函数 */
class CalcNodeSize : public TreeVisit
{
public:
    AC_RET operator()(Tree *node, TREE_VISIT_ORDER order) override;

    uint16_t getSize();

    uint16_t getNum();

private:
    uint16_t _size = 0;
    uint16_t _num = 0;
};

AC_RET CalcNodeSize::operator()(Tree *node, TREE_VISIT_ORDER order)
{
    DataTree *data_node = static_cast<DataTree *>(node);
    if (nullptr == data_node)
    {
        return AC_ERROR;
    }
    if (TREE_PRE_ORDER == order)
    {
        _size += sizeof(DataTree);
        _num++;
    }
    return AC_OK;
}

uint16_t CalcNodeSize::getSize()
{
    return _size;
}

uint16_t CalcNodeSize::getNum()
{
    return _num;
}

/* 创建Data Module树节点的仿函数 */
class CreateDataTreeNode : public TreeVisit
{
public:
    AC_RET operator()(Tree *node, TREE_VISIT_ORDER order) override;

    DataTree *getRes();

private:
    DataTree *_root = nullptr;
    DataTree *_current_node = nullptr;
};

AC_RET CreateDataTreeNode::operator()(Tree *node, TREE_VISIT_ORDER order)
{
    JsonTree *json_node = static_cast<JsonTree *>(node);
    DataTree *data_node = nullptr;
    AC_DATA_TYPE type = AC_UNKNOWN;
    uint16_t size = 0;
    uint16_t array_num = 0;

    if (nullptr == json_node)
    {
        return AC_ERROR;
    }
    /* 后序遍历, 回到当前节点 */
    if (TREE_POST_ORDER == order)
    {
        /* 如果当前节点是数组, 则补齐数组数量 */
        if (AC_ARRAY == _current_node->getType())
        {
            /* 获取数组元素个数 */
            array_num = getArrayNum(_current_node->getKey());

            for (uint16_t i = 1; i < array_num; i++)
            {
                /* 复制已经建立的数组首元素 */
                data_node = DataModule::copyTree(_current_node->getFirstChild());
                NULL_CHECK(data_node);
                /* 添加进数组中 */
                _current_node->addChild(data_node);
            }
        }
        /* 回到父节点 */
        _current_node = _current_node->getParent();
        return AC_OK;
    } else if (TREE_PRE_ORDER == order) /* 前序遍历, 建立新节点 */
    {
        /* 为当前节点分配空间 */
        NULL_CHECK(data_node = DataModule::allocNode());

        if (JSON_TYPE_DATA == json_node->getType())
        {
            data_node->setKey(json_node->getKey());
            size = Type::transStrToType(json_node->getVal(), type);
            data_node->setType(type);
            data_node->setSize(size);
        } else
        {
            if (nullptr != json_node->getParent() && JSON_TYPE_ARRAY == json_node->getParent()->getType())
            {
                data_node->setKey("0");
            } else
            {
                data_node->setKey(json_node->getKey());
            }
            data_node->setType(static_cast<AC_DATA_TYPE>(json_node->getType()));
            data_node->setSize(0);
        }
        if (JSON_TYPE_ROOT == json_node->getType())
        {
            _root = data_node;
            _current_node = data_node;
        } else
        {
            _current_node->addChild(data_node);
            _current_node = data_node;
        }
    }
    return AC_OK;
    error:
    return AC_ERROR;
}

DataTree *CreateDataTreeNode::getRes()
{
    return _root;
}

/* 为Data Module树节点计算数据空间的仿函数 */
class CalcDataSize : public TreeVisit
{
public:
    AC_RET operator()(Tree *node, TREE_VISIT_ORDER order) override;
};

AC_RET CalcDataSize::operator()(Tree *node, TREE_VISIT_ORDER order)
{
    uint16_t align = 0;
    uint16_t remainder = 0;
    uint16_t max_child = 0;
    DataTree *child = nullptr;
    DataTree *data_node = static_cast<DataTree *>(node);

    if (nullptr == data_node)
    {
        return AC_ERROR;
    }

    /* 从下至上计算各层数据结构所占用的空间 */
    if (TREE_PRE_ORDER == order)
    {
        return AC_OK;
    }

    child = data_node->getFirstChild();
    /* 若没有子节点, 则说明是叶节点, 基本类型无需对齐自身; 若节点为数组, 同样无需对齐自身, 直接进入首地址对齐 */
    if (nullptr == child || AC_ARRAY == data_node->getType()) goto align_parent;
    /* 计算子节点中最大的节点大小 */
    while (nullptr != child)
    {
        max_child = max_child > child->getSize() ? max_child : child->getSize();
        child = child->getNeighbor();
    }
    /* 对于本层结构体的自身对齐, 对齐规则: 结构体大小为系统对齐值和最大成员大小中较小的值 */
    align = ALIGN_VALUE < max_child ? ALIGN_VALUE : max_child;

    remainder = data_node->getSize() % align;

    data_node->setSize(data_node->getSize() + remainder);

    align_parent:
    /* 无父节点则为根节点, 计算完成 */
    if (nullptr == data_node->getParent())
    {
        return AC_OK;
    }
    /* 计算首地址对齐, 对齐规则: 对齐值为系统对齐值和元素中较小的值, 元素首地址需为对齐值的整数倍 */
    align = ALIGN_VALUE < data_node->getSize() ? ALIGN_VALUE : data_node->getSize();

    remainder = data_node->getParent()->getSize() % align;

    data_node->getParent()->setSize(data_node->getParent()->getSize() + remainder);

    data_node->setSize(data_node->getSize() + data_node->getParent()->getSize());
    return AC_OK;
}

class AllocDataTreeData : public TreeVisit
{
public:
    AC_RET operator()(Tree *node, TREE_VISIT_ORDER order) override;
};

AC_RET AllocDataTreeData::operator()(Tree *node, TREE_VISIT_ORDER order)
{
    DataTree *data_node = static_cast<DataTree *>(node);
    if (nullptr == data_node)
    {
        return AC_ERROR;
    }
    if (TREE_PRE_ORDER == order)
    {
        if (AC_ARRAY == data_node->getType())
        {
            data_node->allocData(DataModule::alloc(data_node->getSize()));
        }
    }
    return AC_OK;
}

uint8_t *DataModule::_node_head = nullptr;
uint8_t *DataModule::_data_head = nullptr;
uint8_t *DataModule::_ptr = nullptr;
uint32_t DataModule::_size = 0;
DataTree *DataModule::_root = nullptr;

AC_RET DataModule::init(uint32_t size)
{
    _node_head = new uint8_t[size];
    _data_head = _node_head;
    _ptr = _node_head;
    _size = size;
    return AC_OK;
}

AC_RET DataModule::deInit()
{
    delete[] _node_head;
    _node_head = nullptr;
    _data_head = nullptr;
    return AC_OK;
}

void DataModule::clear()
{
    _ptr = _node_head;
    _data_head = _node_head;
    _root = nullptr;
    memset(_node_head, 0, _size);
}

AC_RET DataModule::load()
{
    return AC_OK;
}

AC_RET DataModule::save()
{
    return AC_OK;
}

AC_RET DataModule::create(JsonTree *data)
{
    CreateDataTreeNode create_node;
    CalcDataSize calc_size;
    AllocDataTreeData alloc_data;

    RETURN_CHECK(data->traverse(create_node));
    NULL_CHECK(_root = create_node.getRes());

    _sync();

    RETURN_CHECK(_root->traverse(calc_size));

    RETURN_CHECK(_root->traverse(alloc_data));

    return AC_OK;
    error:
    BASE_ERROR("create data tree error");
    clear();
    return AC_ERROR;
}

AC_RET DataModule::dump(char *buf, uint32_t len)
{
    DumpDataModule dump_module(buf, 200);

    if (nullptr == _root)
    {
        BASE_ERROR("no data");
        return AC_ERROR;
    }
    _root->traverse(dump_module);
    return AC_OK;
}

AC_RET DataModule::set(char *url, JsonTree *data)
{
    return AC_OK;
}

AC_RET DataModule::get(char *url, JsonTree *data)
{
    return AC_OK;
}

AC_RET DataModule::add(char *url, JsonTree *data)
{
    return AC_OK;
}

AC_RET DataModule::del(char *url)
{
    return AC_OK;
}

AC_RET DataModule::read(char *url, void *data, uint16_t size, AC_DATA_TYPE type)
{
    return AC_OK;
}

AC_RET DataModule::write(char *url, void *data, uint16_t size, AC_DATA_TYPE type)
{
    return AC_OK;
}

DataTree *DataModule::allocNode()
{
    DataTree *res = nullptr;
    if (_ptr + sizeof(DataTree) > _node_head + _size)
    {
        BASE_ERROR("no space");
        return nullptr;
    }
    res = new(_ptr) DataTree();
    _ptr += sizeof(DataTree);
    return res;
}

void *DataModule::alloc(uint16_t size)
{
    uint8_t *res = nullptr;
    if (_ptr + size > _data_head + _size)
    {
        BASE_ERROR("no space");
        return nullptr;
    }
    res = _ptr;
    _ptr += size;
    return res;
}

DataTree *DataModule::copyTree(DataTree *src)
{
    DataTree *res = nullptr;
    uint32_t offset = 0;
    uint32_t size = 0;
    uint32_t num = 0;
    CalcNodeSize calc_size;

    /* 计算被拷贝的树节点的数量和总大小 */
    src->traverse(calc_size);
    size = calc_size.getSize();
    num = calc_size.getNum();
    /* 为拷贝的新树申请内存 */
    res = static_cast<DataTree *>(alloc(size));
    if (nullptr == res)
    {
        return nullptr;
    }
    /* 拷贝旧树 */
    memcpy(res, src, size);
    /* 计算新树与旧树的偏移量 */
    offset = res - src;
    /* 重新建立内部链接 */
    for (uint32_t i = 0; i < num; i++)
    {
        if (nullptr != src[i].getFirstChild())
            res[i].setFirstChild(src[i].getFirstChild() + offset);
        if (nullptr != src[i].getNeighbor())
            res[i].setNeighbor(src[i].getNeighbor() + offset);
        if (nullptr != src[i].getParent())
            res[i].setParent(src[i].getParent() + offset);
    }

    return res;
}

void DataModule::_sync()
{
    _data_head = _ptr;
}

DataTree *DataModule::_findNode(char *url)
{
    return nullptr;
}











