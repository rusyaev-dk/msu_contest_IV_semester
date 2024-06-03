#pragma once

#include "SetAbstract.h"
#include "LinkedList1.h"

using namespace std;

class Set : public AbstractSet {
private:
    LinkedList1** _data_array;

    size_t _max_bytes;
    size_t _elem_count;

    void _rehash_set();
    void _rehash_single_elem(LinkedList1** new_data_array, Iterator* list_iter);
    void _rehash_err_cleanup(LinkedList1** new_data_array, Iterator* list_iter);

public:
    class SetIterator : public AbstractSet::Iterator {
    private:
        Set* _set;
        LinkedList1::ListIterator* _list_iterator;
        
        SetIterator(Iterator* iterator, Set* set);
        size_t _get_elem_hash();
        ~SetIterator();
        
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
