#pragma once
#include "SetAbstract.h"
#include "LinkedList1.h"

using namespace std;

class Set : public AbstractSet {
private:
    LinkedList** _set_data;
    
    size_t _set_bytes_size;
    size_t _elem_count;

    Iterator* _newIterator(size_t set_data_index);
    void _rehash_set();

public:
    class SetIterator : public AbstractSet::Iterator {
    private:
        LinkedList::ListIterator* _list_iterator;
        Set* _set;
        size_t _set_data_index;
        SetIterator(Iterator* iterator, Set* set, size_t set_data_index);
    public:
        void* getElement(size_t &size) override;
        bool hasNext() override;
        void goToNext() override;
        bool equals(Iterator *right) override;

        friend class Set;
    };

    Set(MemoryManager &mem);
    ~Set();

    int insert(void* elem, size_t size) override;

    size_t max_bytes() override;
    int size() override;

    Iterator* find(void* elem, size_t size) override;
    Iterator* newIterator() override;
    
    void remove(Iterator *iter) override;
    void clear() override;
    bool empty() override;

    friend class SetTester;
};
