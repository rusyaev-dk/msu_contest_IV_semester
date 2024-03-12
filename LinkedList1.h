#pragma once
#include <iostream>
#include "ListAbstract.h"
#include <list>

const size_t ELEMSIZE = sizeof(int);

using TList = list<void*>;

class LinkedList : public AbstractList {
private:
    TList* m_list;
public:
    LinkedList(MemoryManager &mem) : AbstractList(mem), m_list(new TList()) {}

    ~LinkedList() {}

    int push_front(void *elem, size_t elemSize) { 
        void* newElem = _memory.allocMem(elemSize);
        memcpy(newElem, elem, ELEMSIZE);

        m_list->push_front(newElem);

        return 0;
    }

    void pop_front() { m_list->pop_front(); }

    void* front(size_t &size) {
        size = ELEMSIZE;
        TList::iterator it = m_list->begin();
        return *it;
    }

    class ListIterator: public LinkedList::Iterator {
    public:
        TList* l;
        TList::iterator it;
        
        ListIterator(TList* l): l(l), it(l->begin()) {}

        void *getElement(size_t &size) { size = ELEMSIZE; return *it; }

        bool hasNext() {
            it++;

            if (it == l->end()) {
                --it;
                return false;
            } else {
                --it;
                return true;
            }
        }

        void goToNext() { it++; }

        bool equals(Iterator *right) {
            ListIterator *r = dynamic_cast<ListIterator*>(right);
            return r->it == it;
        }
    };

    int insert(Iterator *iter, void *elem, size_t elemSize) {
        LinkedList::ListIterator* it = dynamic_cast<LinkedList::ListIterator*>(iter);

        m_list->insert(it->it, elem);

        return 0;
    }
    int size() { return m_list->size(); }

    size_t max_bytes() { return 0; }

    Iterator *find(void *elem, size_t size) {
        ListIterator* it = new ListIterator(m_list);
        size_t _size = ELEMSIZE;


        while (it->it != m_list->end()) {
            int t = *((int*)it->getElement(_size));
            int t2 = *((int*)elem);

            // printf("now: %d ?= %d\n", t, t2);

            if (memcmp(it->getElement(_size), elem, _size/2) == 0) {
                return it;
            }

            it->goToNext();
        }
        return nullptr;
    }

    Iterator *newIterator() { return new ListIterator(m_list); }

    void remove(Iterator *iter) {}

    void clear() { m_list->clear(); }

    bool empty() { return m_list->size() == 0; }
};

