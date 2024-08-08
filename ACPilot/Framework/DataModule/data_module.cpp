//
// Created by zhaohe on 2024/1/22.
//
#include <new>
#include <string.h>
#include "data_module.h"
#include "error_handing.h"
#include "Debug/default_debug.h"
#include "Nvs/nvs_driver.h"

using namespace Framework;

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

class DataTreeStructToJson : public Common::TreeVisit
{
public:
    AC_RET operator()(Common::Tree *node, Common::TREE_VISIT_ORDER order) override;

    Utils::JsonTree *getRes();

private:
    Utils::JsonTree *_root = nullptr;
    Utils::JsonTree *_current_node = nullptr;
};

AC_RET DataTreeStructToJson::operator()(Common::Tree *node, Common::TREE_VISIT_ORDER order)
{
    Utils::JsonTree *json_node = nullptr;
    Utils::DataTree *data_node = static_cast<Utils::DataTree *>(node);

    if (nullptr == data_node)
    {
        return AC_ERROR;
    }
    /* 后序遍历, 回到当前节点 */
    if (Common::TREE_POST_ORDER == order)
    {
        /* 如果当前节点是数组, 则仅保留首个孩子节点 */
        if (Utils::JSON_TYPE_ARRAY == _current_node->getType())
        {
            Utils::JsonTree *child = _current_node->getFirstChild();
            Utils::JsonTree *del_node = nullptr;
            NULL_CHECK(child);
            while (nullptr != child->getNeighbor())
            {
                del_node = child->getNeighbor();
                _current_node->delChild(del_node);
                Utils::Json::free(del_node);
            }
        }
        /* 回到父节点 */
        _current_node = _current_node->getParent();
        return AC_OK;
    } else if (Common::TREE_PRE_ORDER == order) /* 前序遍历, 建立新节点 */
    {
        /* 为当前节点分配空间 */
        NULL_CHECK(json_node = Utils::Json::alloc());

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

Utils::JsonTree *DataTreeStructToJson::getRes()
{
    return _root;
}

/* 计算Data Module树(子)节点和数量的仿函数 */
class CalcNodeSize : public Common::TreeVisit
{
public:
    AC_RET operator()(Common::Tree *node, Common::TREE_VISIT_ORDER order) override;

    uint16_t getSize();

    uint16_t getNum();

private:
    uint16_t _size = 0;
    uint16_t _num = 0;
};

AC_RET CalcNodeSize::operator()(Common::Tree *node, Common::TREE_VISIT_ORDER order)
{
    Utils::DataTree *data_node = static_cast<Utils::DataTree *>(node);

    if (nullptr == data_node)
    {
        return AC_ERROR;
    }

    if (Common::TREE_PRE_ORDER == order)
    {
        _size += sizeof(Utils::DataTree);
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
class CreateDataTreeNode : public Common::TreeVisit
{
public:
    AC_RET operator()(Common::Tree *node, Common::TREE_VISIT_ORDER order) override;

    Utils::DataTree *getRes();

private:
    Utils::DataTree *_root = nullptr;
    Utils::DataTree *_current_node = nullptr;
};

AC_RET CreateDataTreeNode::operator()(Common::Tree *node, Common::TREE_VISIT_ORDER order)
{
    Utils::JsonTree *json_node = static_cast<Utils::JsonTree *>(node);
    Utils::DataTree *data_node = nullptr;
    uint16_t array_num = 0;

    if (nullptr == json_node)
    {
        return AC_ERROR;
    }
    /* 后序遍历, 回到当前节点 */
    if (Common::TREE_POST_ORDER == order)
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
    } else if (Common::TREE_PRE_ORDER == order) /* 前序遍历, 建立新节点 */
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

Utils::DataTree *CreateDataTreeNode::getRes()
{
    return _root;
}

/* 为Data Module树节点计算数据空间的仿函数 */
class CalcDataSize : public Common::TreeVisit
{
public:
    AC_RET operator()(Common::Tree *node, Common::TREE_VISIT_ORDER order) override;
};

AC_RET CalcDataSize::operator()(Common::Tree *node, Common::TREE_VISIT_ORDER order)
{
    Utils::DataTree *data_node = static_cast<Utils::DataTree *>(node);

    if (nullptr == data_node)
    {
        return AC_ERROR;
    }
    /* 从下至上计算各层数据结构所占用的空间, 暂不考虑内存对齐 */
    if (Common::TREE_PRE_ORDER == order)
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

class AllocDataTreeData : public Common::TreeVisit
{
public:
    AC_RET operator()(Common::Tree *node, Common::TREE_VISIT_ORDER order) override;
};

AC_RET AllocDataTreeData::operator()(Common::Tree *node, Common::TREE_VISIT_ORDER order)
{
    Utils::DataTree *data_node = static_cast<Utils::DataTree *>(node);
    if (nullptr == data_node)
    {
        return AC_ERROR;
    }
    if (Common::TREE_POST_ORDER == order)
    {
        if (nullptr == data_node->getFirstChild())
        {
            RETURN_CHECK(data_node->allocData(DataModule::alloc(data_node->getSize())));
        } else
        {
            RETURN_CHECK(data_node->allocData(data_node->getFirstChild()->getData()));
        }
    }
    return AC_OK;
    error:
    BASE_ERROR("alloc data error");
    return AC_ERROR;
}

uint8_t *DataModule::_head = nullptr;
uint8_t *DataModule::_node_head = nullptr;
uint8_t *DataModule::_data_head = nullptr;
uint8_t *DataModule::_ptr = nullptr;
uint32_t DataModule::_size = 0;
uint32_t DataModule::_node_size = 0;
uint32_t DataModule::_data_size = 0;
Utils::DataTree *DataModule::_root = nullptr;

AC_RET DataModule::init()
{
    _head = new uint8_t[DATA_MODULE_MEMORY_SIZE];

    memset(_head, 0, DATA_MODULE_MEMORY_SIZE);

    _node_head = _head + sizeof(uint8_t *) + sizeof(_node_size) + sizeof(_data_size);
    _data_head = _node_head;
    _ptr = _node_head;
    _size = DATA_MODULE_MEMORY_SIZE - sizeof(uint8_t *) - sizeof(_node_size) - sizeof(_data_size);
    load();
    return AC_OK;
}

AC_RET DataModule::deInit()
{
    delete[] _node_head;
    _node_head = nullptr;
    _data_head = nullptr;
    return AC_OK;
}

AC_RET DataModule::load()
{
    Driver::Nvs *fd = Driver::Nvs::open(DATA_MODULE_NVS_NAMESPACE);
    uint8_t *old_head = nullptr;
    int32_t offset = 0;

    NULL_CHECK(fd);
    RETURN_CHECK(fd->read(DATA_MODULE_NVS_KEY, _head, DATA_MODULE_MEMORY_SIZE));

    memcpy(&old_head, _head, sizeof(uint8_t *));
    memcpy(&_node_size, _head + sizeof(uint8_t *), sizeof(_node_size));
    memcpy(&_data_size, _head + sizeof(uint8_t *) + sizeof(_node_size), sizeof(_data_size));

    _node_head = _head + sizeof(uint8_t *) + sizeof(_node_size) + sizeof(_data_size);
    _data_head = _node_head + _node_size;
    _ptr = _data_head + _data_size;
    _root = static_cast<Utils::DataTree *>((void *) _node_head);

    Driver::Nvs::close(fd);

    offset = _head - old_head;

    for (uint32_t i = 0; i < _node_size / sizeof(Utils::DataTree); ++i)
    {
        if (nullptr != _root[i].getParent())
        {
            _root[i].setParent((Utils::DataTree *) ((uint8_t *) _root[i].getParent() + offset));
        }
        if (nullptr != _root[i].getFirstChild())
        {
            _root[i].setFirstChild((Utils::DataTree *) ((uint8_t *) _root[i].getFirstChild() + offset));
        }
        if (nullptr != _root[i].getNeighbor())
        {
            _root[i].setNeighbor((Utils::DataTree *) ((uint8_t *) _root[i].getNeighbor() + offset));
        }
        if (nullptr != _root[i].getData())
        {
            _root[i].setData((uint8_t *) _root[i].getData() + offset);
        }
    }
    return AC_OK;
    error:
    BASE_ERROR("load data error");
    Driver::Nvs::close(fd);
    return AC_ERROR;
}

AC_RET DataModule::save()
{
    Driver::Nvs *fd = Driver::Nvs::open(DATA_MODULE_NVS_NAMESPACE);
    NULL_CHECK(fd);

    memcpy(_head, &_head, sizeof(uint8_t *));
    memcpy(_head + sizeof(uint8_t *), &_node_size, sizeof(_node_size));
    memcpy(_head + sizeof(uint8_t *) + sizeof(_node_size), &_data_size, sizeof(_data_size));

    RETURN_CHECK(fd->write(DATA_MODULE_NVS_KEY, _head, DATA_MODULE_MEMORY_SIZE));
    RETURN_CHECK(fd->save());

    Driver::Nvs::close(fd);

    return AC_OK;
    error:
    BASE_ERROR("save data error");
    Driver::Nvs::close(fd);
    return AC_ERROR;
}

AC_RET DataModule::reset()
{
    _ptr = _node_head;
    _data_head = _node_head;
    _root = nullptr;
    _node_size = 0;
    _data_size = 0;
    memset(_head, 0, DATA_MODULE_MEMORY_SIZE);
    return AC_OK;
}

AC_RET DataModule::create(Utils::JsonTree *data)
{
    CreateDataTreeNode create_node;
    CalcDataSize calc_size;
    AllocDataTreeData alloc_data;
    reset();

    RETURN_CHECK(data->traverse(create_node));
    NULL_CHECK(_root = create_node.getRes());

    _node_size = _ptr - _node_head;
    _sync();

    RETURN_CHECK(_root->traverse(calc_size));

    RETURN_CHECK(_root->traverse(alloc_data));

    _data_size = _ptr - _data_head;

    return AC_OK;
    error:
    BASE_ERROR("create data tree error");
    reset();
    return AC_ERROR;
}

AC_RET DataModule::dumpStruct(char *buf, uint32_t len)
{
    DataTreeStructToJson struct_to_json;
    Utils::JsonTree *ret = nullptr;

    NULL_CHECK(_root);

    _root->traverse(struct_to_json);

    NULL_CHECK(ret = struct_to_json.getRes());

    RETURN_CHECK(Utils::Json::serialize(ret, buf, len));

    Utils::Json::free(ret);

    return AC_OK;
    error:
    BASE_ERROR("dump struct error");
    if (nullptr != ret)
    {
        Utils::Json::free(ret);
    }
    return AC_ERROR;
}

AC_RET DataModule::dumpData(char *buf, uint32_t len)
{
    Utils::JsonTree *ret = nullptr;

    NULL_CHECK(_root);

    NULL_CHECK(ret = Utils::DataTree::toJson(_root));

    RETURN_CHECK(Utils::Json::serialize(ret, buf, len));

    Utils::Json::free(ret);

    return AC_OK;
    error:
    BASE_ERROR("dump data error");
    if (nullptr != ret)
    {
        Utils::Json::free(ret);
    }
    return AC_ERROR;
}

AC_RET DataModule::set(const char *url, Utils::JsonTree *data)
{
    if (nullptr == _root || nullptr == data || nullptr == url)
    {
        BASE_ERROR("param error");
        return AC_ERROR;
    }
    Utils::DataTree *node = Utils::DataTree::find(_root, url);

    if (nullptr == node)
    {
        return AC_ERROR;
    }
    return Utils::DataTree::fromJson(node, data->getFirstChild());
}

AC_RET DataModule::get(const char *url, Utils::JsonTree *data)
{
    if (nullptr == _root || nullptr == data || nullptr == url)
    {
        BASE_ERROR("param error");
        return AC_ERROR;
    }
    Utils::DataTree *node = Utils::DataTree::find(_root, url);
    if (nullptr == node)
    {
        return AC_OK;
    }
    data->addChild(Utils::DataTree::toJson(node));
    return AC_OK;
}

AC_RET DataModule::add(const char *url, Utils::JsonTree *data)
{
    if (nullptr == _root || nullptr == data || nullptr == url)
    {
        BASE_ERROR("param error");
        return AC_ERROR;
    }

    Utils::JsonTree *json_node = data->getFirstChild();
    Utils::DataTree *data_node = Utils::DataTree::find(_root, url);
    Utils::JsonTree *json_child = nullptr;
    Utils::DataTree *data_child = nullptr;

    uint16_t index = 1;

    if (nullptr == data_node)
    {
        return AC_ERROR;
    }

    if (nullptr == json_node)
    {
        return AC_OK;
    }

    if (AC_ARRAY != data_node->getType() || Utils::JSON_TYPE_ARRAY != json_node->getType())
    {
        BASE_ERROR("node is not array");
        return AC_ERROR;
    }

    json_child = json_node->getFirstChild();
    data_child = data_node->getFirstChild();

    while (nullptr != json_child)
    {
        while (nullptr != data_child)
        {
            if (Utils::DataTree::dataArrayNodeIsUsed(data_child))
            {
                data_child = data_child->getNeighbor();
                index++;
            } else
            {
                break;
            }
        }

        if (nullptr == data_child)
        {
            BASE_ERROR("no space");
            return AC_ERROR;
        }

        Utils::JsonTree::jsonArrayNodeSetIndex(json_child, 0);
        if (AC_OK != Utils::DataTree::fromJson(data_child, json_child))
        {
            BASE_ERROR("fromJson error");
            data_child->clearData();
            return AC_ERROR;
        }
        Utils::DataTree::dataArrayNodeSetIndex(data_child, index);
        json_child = json_child->getNeighbor();
    }
    return AC_OK;
}

AC_RET DataModule::del(const char *url)
{
    if (nullptr == _root || nullptr == url)
    {
        BASE_ERROR("param error");
        return AC_ERROR;
    }

    Utils::DataTree *data_node = Utils::DataTree::find(_root, url);
    Utils::DataTree *data_neighbor = nullptr;
    uint16_t index = 0;

    if (data_node == nullptr || data_node->getParent() == nullptr || data_node->getParent()->getType() != AC_ARRAY)
    {
        BASE_ERROR("node not found");
        return AC_ERROR;
    }

    index = Utils::DataTree::dataArrayNodeGetIndex(data_node);

    data_neighbor = data_node->getNeighbor();

    while (nullptr != data_neighbor && Utils::DataTree::dataArrayNodeIsUsed(data_neighbor))
    {
        RETURN_CHECK(data_node->copyData(data_neighbor));
        Utils::DataTree::dataArrayNodeSetIndex(data_node, index);
        index++;
        data_node = data_neighbor;
        data_neighbor = data_node->getNeighbor();
    }

    data_node->clearData();
    Utils::DataTree::dataArrayNodeSetIndex(data_node, 0);

    return AC_OK;
    error:
    BASE_ERROR("del error");
    return AC_ERROR;
}

AC_RET DataModule::create(char *data)
{
    Utils::JsonTree *json = nullptr;
    AC_RET ret = AC_OK;

    json = Utils::Json::deserialize(data);

    if (nullptr == json)
    {
        BASE_ERROR("deserialize error");
        return AC_ERROR;
    }

    ret = create(json);

    Utils::Json::free(json);

    return ret;
}

AC_RET DataModule::set(const char *url, char *data)
{
    Utils::JsonTree *json = nullptr;
    AC_RET ret = AC_OK;

    json = Utils::Json::deserialize(data);

    if (nullptr == json)
    {
        BASE_ERROR("deserialize error");
        return AC_ERROR;
    }

    ret = set(url, json);

    Utils::Json::free(json);

    return ret;
}

AC_RET DataModule::get(const char *url, char *data, uint32_t len)
{
    Utils::JsonTree *json = nullptr;

    json = Utils::Json::alloc();

    if (nullptr == json)
    {
        BASE_ERROR("alloc error");
        return AC_ERROR;
    }

    json->setType(Utils::JSON_TYPE_ROOT);

    if (AC_OK != get(url, json))
    {
        BASE_ERROR("get error");
        Utils::Json::free(json);
        return AC_ERROR;
    }

    if (AC_OK != Utils::Json::serialize(json, data, len))
    {
        BASE_ERROR("serialize error");
        Utils::Json::free(json);
        return AC_ERROR;
    }

    Utils::Json::free(json);
    return AC_OK;
}

AC_RET DataModule::add(const char *url, char *data)
{
    Utils::JsonTree *json = nullptr;
    AC_RET ret = AC_OK;

    json = Utils::Json::deserialize(data);

    if (nullptr == json)
    {
        BASE_ERROR("deserialize error");
        return AC_ERROR;
    }

    ret = add(url, json);

    Utils::Json::free(json);

    return ret;
}

AC_RET DataModule::read(const char *url, void *data, uint16_t size)
{
    Utils::DataTree *node = nullptr;

    if (nullptr == _root || nullptr == data || nullptr == url)
    {
        BASE_ERROR("param error");
        return AC_ERROR;
    }

    node = Utils::DataTree::find(_root, url);

    if (nullptr == node)
    {
        BASE_ERROR("url[%s] not found", url);
        return AC_ERROR;
    }

    if (nullptr == node->getData())
    {
        BASE_ERROR("null ptr data");
        return AC_ERROR;
    }

    if (node->getSize() != size)
    {
        BASE_ERROR("size not match");
        return AC_ERROR;
    }

    memcpy(data, node->getData(), size);
    return AC_OK;
}

AC_RET DataModule::registerAction(const char *url, TaskFunction action)
{
    Utils::DataTree *node = nullptr;

    if (nullptr == _root || nullptr == url || nullptr == action)
    {
        BASE_ERROR("param error");
        return AC_ERROR;
    }
    node = Utils::DataTree::find(_root, url);

    if (nullptr == node)
    {
        BASE_ERROR("url[%s] not found", url);
    }

    node->setData((void *) action);

    return AC_OK;
}

AC_RET DataModule::doAction(const char *url)
{
    Utils::DataTree *node = nullptr;

    if (nullptr == _root || nullptr == url)
    {
        BASE_ERROR("param error");
        return AC_ERROR;
    }
    node = Utils::DataTree::find(_root, url);

    if (nullptr == node)
    {
        BASE_ERROR("url[%s] not found", url);
    }

    TaskFunction func = (TaskFunction) node->getData();

    if (nullptr == func)
    {
        BASE_ERROR("action is NULL");
        return AC_ERROR;
    }
    func(nullptr);
    return AC_OK;
}

AC_RET DataModule::write(const char *url, void *data, uint16_t size)
{
    Utils::DataTree *node = nullptr;

    if (nullptr == _root || nullptr == data || nullptr == url)
    {
        BASE_ERROR("param error");
        return AC_ERROR;
    }

    node = Utils::DataTree::find(_root, url);

    if (nullptr == node)
    {
        BASE_ERROR("url[%s] not found", url);
        return AC_ERROR;
    }

    if (nullptr == node->getData())
    {
        BASE_ERROR("null ptr data");
        return AC_ERROR;
    }

    if (node->getSize() != size)
    {
        BASE_ERROR("size not match");
        return AC_ERROR;
    }

    memcpy(node->getData(), data, size);
    return AC_OK;
}

AC_RET DataModule::info(char *buf, uint32_t len)
{
    snprintf(buf, len, "DATA MODULE INFO:\n"
                       "\ttotal   free    node    data\n"
                       "\t%-8lu%-8lu%-8lu%-8lu\n",
             _size,
             _size - (uint32_t) (_ptr - _node_head),
             (uint32_t) (_data_head - _node_head),
             (uint32_t) (_ptr - _data_head));
    return AC_OK;
}

Utils::DataTree *DataModule::allocNode()
{
    Utils::DataTree *res = nullptr;

    if (_ptr + sizeof(Utils::DataTree) > _node_head + _size)
    {
        BASE_ERROR("no space");
        return nullptr;
    }

    res = new(_ptr) Utils::DataTree();
    _ptr += sizeof(Utils::DataTree);
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

Utils::DataTree *DataModule::copyTree(Utils::DataTree *src)
{
    Utils::DataTree *res = nullptr;
    uint32_t offset = 0;
    uint16_t size = 0;
    uint16_t num = 0;
    CalcNodeSize calc_size;

    /* 计算被拷贝的树节点的数量和总大小 */
    src->traverse(calc_size);
    size = calc_size.getSize();
    num = calc_size.getNum();
    /* 为拷贝的新树申请内存 */
    res = static_cast<Utils::DataTree *>(alloc(size));
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





