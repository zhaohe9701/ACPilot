/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2023-02-23 22:52:24
 * @LastEditors: zhaohe
 * @LastEditTime: 2023-02-24 00:05:22
 * @FilePath: \ZH_FLIGHT\Sys\Common\ac_list.h
 * Copyright (C) 2023 zhaohe. All rights reserved.
 */
#ifndef AC_LIST_H_
#define AC_LIST_H_

template <class T>
class List;

template <class T>
class ListNode
{
friend class List<T>;
private:
    ListNode<T> *_prev = nullptr;
    ListNode<T> *_next = nullptr;
public:
    T data;
    ListNode<T> *getNext();
    ListNode<T> *getPrev();
    T& operator*();
};

template <class T>
class List
{
private:
    ListNode<T> *_head = nullptr;
public:
    List();
    void pushBack(T &data);
    void pushFront(T &data);
    ListNode<T> *begin();
    ListNode<T> *end();
    ListNode<T> *index(int ind);
    ListNode<T> *operator[](int ind);
    ~List();
};

template <class T>
ListNode<T> *ListNode<T>::getNext()
{
    return _next;
}

template <class T>
ListNode<T> *ListNode<T>::getPrev()
{
    return _prev;
}

template <class T>
T& ListNode<T>::operator*()
{
    return data;
}

template <class T>
List<T>::List()
{
    _head = new ListNode<T>();
    _head->_next = _head;
    _head->_prev = _head;
}

template <class T>
void List<T>::pushBack(T &data)
{
    ListNode<T> *node = new ListNode<T>();
    node->data = data;
    node->_next = _head;
    node->_prev = _head->_prev;
    _head->_prev->_next = node;
    _head->_prev = node;
}

template <class T>
void List<T>::pushFront(T &data)
{
    ListNode<T> *node = new ListNode<T>();
    node->data = data;
    node->_prev = _head;
    node->_next = _head->_next;
    _head->_next->_prev = node;
    _head->_next = node;
}

template <class T>
ListNode<T> *List<T>::begin()
{
    return _head->_next;
}

template <class T>
ListNode<T> *List<T>::end()
{
    return _head;
}

template <class T>
List<T>::~List()
{
    ListNode<T> *node = _head->getNext();
    ListNode<T> *next_node = nullptr;
    while (node != _head)
    {
        next_node = node->getNext();
        delete node;
        node = next_node;
    }
    delete _head;
}

template<class T>
ListNode<T> *List<T>::index(int ind)
{
    int i = 0;
    ListNode<T> *node = _head->getNext();
    while (node != _head)
    {
        if (i == ind)
        {
            return node;
        }
        if (ind > 0)
        {
            node = node->getNext();
            i++;
        } else
        {
            node = node->getPrev();
            i--;
        }
    }
    return nullptr;
}

template<class T>
ListNode<T> *List<T>::operator[](int ind)
{
    int i = 0;
    ListNode<T> *node = _head->getNext();
    while (node != _head)
    {
        if (i == ind)
        {
            return node;
        }
        if (ind > 0)
        {
            node = node->getNext();
            i++;
        } else
        {
            node = node->getPrev();
            i--;
        }
    }
    return nullptr;
}

#endif