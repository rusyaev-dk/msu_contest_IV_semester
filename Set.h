#pragma once

#include "SetAbstract.h"
#include "LinkedList1.h"

using namespace std;

class Set : public AbstractSet {
private:
    LinkedList1** _data_array;

    size_t _bytes_size;
    size_t _elem_count;

    void _rehash_set();

public:
    class SetIterator : public AbstractSet::Iterator {
    private:
        LinkedList1::ListIterator* _list_iterator;
        Set* _set;
        
        SetIterator(Iterator* iterator, Set* set);
        size_t _get_data_index();
        // добавить деструктор
    public:
        void* getElement(size_t &size) override;
        bool hasNext() override;
        void goToNext() override;
        bool equals(Iterator *right) override;

        friend class Set;
    };

    Set(MemoryManager &mem);

    int insert(void* elem, size_t size) override;

    Iterator* find(void* elem, size_t size) override;
    Iterator* newIterator() override;
    
    void remove(Iterator *iter) override;
    void clear() override;
    
    bool empty() override;
    int size() override;
    size_t max_bytes() override;
    
    ~Set();

    friend class SetTester;
};
