#pragma once
#include <iostream>
#include "ListAbstract.h"
#include <list>

typedef struct Pair {
    Pair(size_t elemSize, void* elem): elemSize(elemSize), elem(elem) {}
    size_t elemSize;
    void* elem;
} Pair;

using TList = list<Pair *>;

class LinkedList1 : public AbstractList
{
private:
    TList *m_list;

public:
    LinkedList1(MemoryManager &mem) : AbstractList(mem), m_list(new TList()) {}

    ~LinkedList1() {}

    int push_front(void *elem, size_t elemSize)
    {
        void *newElem = _memory.allocMem(elemSize);
        memcpy(newElem, elem, elemSize);

        m_list->push_front(new Pair(elemSize, newElem));

        return 0;
    }

    void pop_front() { m_list->pop_front(); }

    void *front(size_t &size) {
        TList::iterator it = m_list->begin();
        size = (*it)->elemSize;
        return (*it)->elem;
    }

    class ListIterator : public LinkedList1::Iterator {
    public:
        TList* l;
        TList::iterator it;

        ListIterator(TList* l) : l(l), it(l->begin()) {}
        ListIterator(TList* l, TList::iterator it) : l(l), it(it) {}

        void *getElement(size_t &size) {
            if (it == l->end()) return nullptr;
            size = (*it)->elemSize;
            return (*it)->elem;
        }

        bool hasNext() {
            if (it == l->end()) return false;
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

        bool equals(Iterator *right) {
            ListIterator *r = dynamic_cast<ListIterator *>(right);
            return r->it == it;
        }
    };

    int insert(Iterator *iter, void *elem, size_t elemSize) { return 0; }

    int size() { return m_list->size(); }

    size_t max_bytes() { return 0; }

    Iterator *find(void *elem, size_t size) {
        TList::iterator it = m_list->begin();

        void* _elem;
        for (; it != m_list->end(); ++it) {
            if ((*it)->elemSize == size) {
                _elem = (*it)->elem;
                if (memcmp(_elem, elem, size) == 0) {
                    return new ListIterator(m_list, it);
                }

            }
        }

        return nullptr;
    }

    Iterator *newIterator() { return new ListIterator(m_list); }

    void remove(Iterator *iter) {
        ListIterator *it = dynamic_cast<ListIterator *>(iter);
        it->it = m_list->erase(it->it);
    }
    // void remove(Iterator *iter){
    //     size_t size1, size2;
    //     void* toFind = iter->getElement(size1);

    //     if (m_list->size() == 0) return;

    //     ListIterator *it = dynamic_cast<ListIterator *>(newIterator());
    //     void* cur;

    //     while (it) {
    //         cur = it->getElement(size2);
    //         if (!cur) break;

    //         if (size1 != size2) continue;

    //         int flag = memcmp(toFind, cur, size2);

    //         // TList::iterator newIt = m_list->begin();
    //         // while (newIt != it->it) newIt++;
    //         // newIt++;

    //         if (flag == 0) {
    //             it->it = 
    //             m_list->remove(*(it->it));
    //             return;
    //         }
    //         it->goToNext();
    //     }
    // }

    void clear() { m_list->clear(); }

    bool empty() { return m_list->size() == 0; }
};
