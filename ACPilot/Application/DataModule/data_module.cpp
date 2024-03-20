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

class DataTreeStructToJson : public TreeVisit
{
public:
    AC_RET operator()(Tree *node, TREE_VISIT_ORDER order) override;
    JsonTree *getRes();
private:
    JsonTree *_root = nullptr;
    JsonTree *_current_node = nullptr;
};

AC_RET DataTreeStructToJson::operator()(Tree *node, TREE_VISIT_ORDER order)
{
    JsonTree *json_node = nullptr;
    DataTree *data_node = static_cast<DataTree *>(node);

    if (nullptr == data_node)
    {
        return AC_ERROR;
    }
    /* 后序遍历, 回到当前节点 */
    if (TREE_POST_ORDER == order)
    {
        /* 如果当前节点是数组, 则仅保留首个孩子节点 */
        if (JSON_TYPE_ARRAY == _current_node->getType())
        {
            JsonTree *child = _current_node->getFirstChild();
            JsonTree *del_node = nullptr;
            NULL_CHECK(child);
            while (nullptr != child->getNeighbor())
            {
                del_node = child->getNeighbor();
                _current_node->delChild(del_node);
                Json::free(del_node);
            }
        }
        /* 回到父节点 */
        _current_node = _current_node->getParent();
        return AC_OK;
    } else if (TREE_PRE_ORDER == order) /* 前序遍历, 建立新节点 */
    {
        /* 为当前节点分配空间 */
        NULL_CHECK(json_node = Json::alloc());

        data_node->singleNodeToStruct(json_node);

        if (nullptr == _root)
        {
            _root = json_node;
            _current_node = json_node;
        } else
        {
            _current_node->addChild(json_node);
            _current_node = json_node;
        }
    }
    return AC_OK;
    error:
    _root = nullptr;
    return AC_ERROR;
}

JsonTree *DataTreeStructToJson::getRes()
{
    return _root;
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

        RETURN_CHECK(data_node->singleNodeFromStruct(json_node));

        if (nullptr == _root)
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
    _root = nullptr;
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
    DataTree *data_node = static_cast<DataTree *>(node);

    if (nullptr == data_node)
    {
        return AC_ERROR;
    }
    /* 从下至上计算各层数据结构所占用的空间, 暂不考虑内存对齐 */
    if (TREE_PRE_ORDER == order)
    {
        return AC_OK;
    }

    if (nullptr == data_node->getParent())
    {
        return AC_OK;
    }

    data_node->getParent()->setSize(data_node->getParent()->getSize() + data_node->getSize());
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
        if (nullptr == data_node->getFirstChild())
        {
            data_node->allocData(DataModule::alloc(data_node->getSize()));
        } else
        {
            data_node->allocData(data_node->getFirstChild()->getData());
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

    memset(_node_head, 0, size);

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

AC_RET DataModule::dumpStruct(char *buf, uint32_t len)
{
    DataTreeStructToJson struct_to_json;
    JsonTree *ret = nullptr;

    NULL_CHECK(_root);

    _root->traverse(struct_to_json);

    NULL_CHECK(ret = struct_to_json.getRes());

    RETURN_CHECK(Json::serialize(ret, buf, len));

    Json::free(ret);

    return AC_OK;
    error:
    BASE_ERROR("dump struct error");
    if (nullptr != ret)
    {
        Json::free(ret);
    }
    return AC_ERROR;
}

AC_RET DataModule::dumpData(char *buf, uint32_t len)
{
    JsonTree *ret = nullptr;

    NULL_CHECK(_root);

    NULL_CHECK(ret = DataTree::toJson(_root));

    RETURN_CHECK(Json::serialize(ret, buf, len));

    Json::free(ret);

    return AC_OK;
    error:
    BASE_ERROR("dump data error");
    if (nullptr != ret)
    {
        Json::free(ret);
    }
    return AC_ERROR;
}

AC_RET DataModule::set(char *url, JsonTree *data)
{
    if (nullptr == _root || nullptr == data || nullptr == url)
    {
        BASE_ERROR("param error");
        return AC_ERROR;
    }
    DataTree *node = DataTree::find(_root, url);

    if (nullptr == node)
    {
        return AC_ERROR;
    }
    return DataTree::fromJson(node, data->getFirstChild());
}

AC_RET DataModule::get(char *url, JsonTree *data)
{
    if (nullptr == _root || nullptr == data || nullptr == url)
    {
        BASE_ERROR("param error");
        return AC_ERROR;
    }
    DataTree *node = DataTree::find(_root, url);
    if (nullptr == node)
    {
        return AC_OK;
    }
    data->addChild(DataTree::toJson(node));
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

AC_RET DataModule::info(char *buf, uint32_t len)
{
    snprintf(buf, len, "DATA MODULE INFO:\n"
                       "\ttotal: %luB\n"
                       "\tfree : %luB\n"
                       "\tused :-node size:%luB\n"
                       "\t      -data size:%luB\n",
                       _size,
                       _size - (uint32_t)(_ptr - _node_head),
                       (uint32_t)(_data_head - _node_head),
                       (uint32_t)(_ptr - _data_head));
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
    uint16_t size = 0;
    uint16_t num = 0;
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
    res->setNeighbor(nullptr);
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











