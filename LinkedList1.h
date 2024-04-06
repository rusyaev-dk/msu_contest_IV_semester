#pragma once
#include <iostream>
#include "ListAbstract.h"
#include <list>

// typedef struct Pair {
//     Pair(size_t elemSize, void* elem): elemSize(elemSize), elem(elem) {}
//     size_t elemSize;
//     void* elem;
// } Pair;

// using TList = list<Pair *>;

// class LinkedList : public AbstractList
// {
// private:
//     TList *m_list;

// public:
//     LinkedList(MemoryManager &mem) : AbstractList(mem), m_list(new TList()) {}

//     ~LinkedList() {}

//     int push_front(void *elem, size_t elemSize)
//     {
//         void *newElem = _memory.allocMem(elemSize);
//         memcpy(newElem, elem, elemSize);

//         m_list->push_front(new Pair(elemSize, newElem));

//         return 0;
//     }

//     void pop_front() { m_list->pop_front(); }

//     void *front(size_t &size) {
//         TList::iterator it = m_list->begin();
//         size = (*it)->elemSize;
//         return (*it)->elem;
//     }

//     class ListIterator : public LinkedList::Iterator {
//     public:
//         TList *l;
//         TList::iterator it;

//         ListIterator(TList *l) : l(l), it(l->begin()) {}

//         void *getElement(size_t &size) {
//             size = (*it)->elemSize;
//             return (*it)->elem;
//         }

//         bool hasNext() {
//             if (l->size() <= 1) return false;

//             TList::iterator prev, cur = l->begin();

//             while (cur != l->end()) {
//                 prev = cur;
//                 cur++;
//             }
//             if (it == prev) return false;

//             return true;
//         }

//         void goToNext() { it++; }

//         bool equals(Iterator *right) {
//             ListIterator *r = dynamic_cast<ListIterator *>(right);
//             return r->it == it;
//         }
//     };

//     int insert(Iterator *iter, void *elem, size_t elemSize) { return 0; }

//     int size() { return m_list->size(); }

//     size_t max_bytes() { return 0; }

//     Iterator *find(void *elem, size_t size) {
//         ListIterator *it = new ListIterator(m_list);

//         size_t _size;
//         void* _elem;
//         while (it->it != m_list->end()) {
//             _elem = it->getElement(_size);

//             if (_size == size && !memcmp(_elem, elem, _size)) return it;

//             it->goToNext();
//         }
//         return nullptr;
//     }

//     Iterator *newIterator() { return new ListIterator(m_list); }

//     void remove(Iterator *iter){
//         if (m_list->size() == 0) return;
//         size_t size1, size2;

//         ListIterator *it = dynamic_cast<ListIterator *>(newIterator());

//         while (it) {
//             void *elem = it->getElement(size1);
//             int flag = memcmp(elem, iter->getElement(size2), min(size1, size2));

//             if (it->hasNext()) it->goToNext(); else it = nullptr;

//             if (!flag && size1 == size2) {
//                 m_list->remove(*(it->it));
//                 return;
//             }
//         }
//     }

//     void clear() { m_list->clear(); }

//     bool empty() { return m_list->size() == 0; }
// };


typedef struct Pair {
    Pair(size_t elemSize, void* elem) : elemSize(elemSize), elem(elem) {}
    size_t elemSize;
    void* elem;
} Pair;

using TList = list<Pair*>;

class LinkedList : public AbstractList
{
private:
    TList* m_list;
    int m_count;

public:
    LinkedList(MemoryManager& mem) : AbstractList(mem), m_list(new TList()) {}

    ~LinkedList() {}

    int push_front(void* elem, size_t elemSize)
    {
        void* newElem = _memory.allocMem(elemSize);
        memcpy(newElem, elem, elemSize);

        m_list->push_front(new Pair(elemSize, newElem));
        m_count++;
        return 0;
    }

    void pop_front() { 
        m_list->pop_front();
        m_count--;
    }

    void* front(size_t& size) {
        TList::iterator it = m_list->begin();
        size = (*it)->elemSize;
        return (*it)->elem;
    }

    class ListIterator : public LinkedList::Iterator {
    public:
        TList* l;
        TList::iterator it;

        ListIterator(TList* l) : l(l), it(l->begin()) {}

        void* getElement(size_t& size) {
            size = (*it)->elemSize;
            return (*it)->elem;
        }

        bool hasNext() {
            if (l->size() <= 1) return false;

            TList::iterator prev, cur = l->begin();

            while (cur != l->end()) {
                prev = cur;
                cur++;
            }
            if (it == prev) return false;

            return true;
        }

        void goToNext() { it++; }

        bool equals(Iterator* right) {
            ListIterator* r = dynamic_cast<ListIterator*>(right);
            return r->it == it;
        }
    };

    int insert(Iterator* iter, void* elem, size_t elemSize) {
        m_count++;
        return 0; 
    }

    int size() { return m_list->size(); }

    size_t max_bytes() { return 0; }

    Iterator* find(void* elem, size_t size) {
        ListIterator* it = new ListIterator(m_list);

        size_t _size;
        void* _elem;
        while (it->it != m_list->end()) {
            _elem = it->getElement(_size);

            if (_size == size && !memcmp(_elem, elem, _size)) return it;

            it->goToNext();
        }
        return nullptr;
    }

    Iterator* newIterator() { return new ListIterator(m_list); }

    void remove(Iterator* iter) {
        if (m_list->size() == 0) return;
        size_t size1, size2;

        ListIterator* it = dynamic_cast<ListIterator*>(newIterator());

        while (it) {
            void* elem = it->getElement(size1);
            int flag = memcmp(elem, iter->getElement(size2), min(size1, size2));

            if (it->hasNext()) it->goToNext(); else it = nullptr;

            if (!flag && size1 == size2) {
                m_list->remove(*(it->it));
                m_count--;
                return;
            }
        }
    }

    void clear() { m_list->clear(); }

    bool empty() { return m_list->size() == 0; }

    int getCount() const { return m_count; }
};
