#pragma once
#include "SetAbstract.h"
#include <list>

using namespace std;

class Set : public AbstractSet {
private:
    list<void*>** nodes;

public:
    class SetIterator : public AbstractSet::Iterator {
        public:
            void* getElement(size_t &size) {}
            bool hasNext() {}
            void goToNext() {}
            bool equals(Iterator *right) {}
    };

    size_t max_bytes() {}

    int size() {}

    Iterator* find(void* elem, size_t size) {}
    Iterator* newIterator() {}

    void remove(Iterator *iter) {}
    void clear() {}
    bool empty() {}
    Set(MemoryManager &mem);

    int insert(void* elem, size_t size) override;

    ~Set();
};