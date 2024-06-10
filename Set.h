#pragma once

#include "SetAbstract.h"

using namespace std;

class Set : public AbstractSet {
private:
    void _rehash_set();
    void _rehash_single_elem(LinkedList1** new_data_array, Iterator* list_iter);
    void _rehash_err_cleanup(LinkedList1** new_data_array, Iterator* list_iter);

public:
    class SetIterator : public AbstractSet::Iterator {
    private:
        Set* _set;
        LinkedList1::ListIterator* _list_iter;
        
        SetIterator(Iterator* iterator, Set* set);
        inline size_t _get_elem_hash() { return _set->_iter_get_elem_hash(_list_iter); };
        
    public:
        inline void* getElement(size_t &size) override { return _set->_iterGetElement(size, _list_iter); };
        inline bool hasNext() override { return _set->_iterHasNext(_list_iter); };
        void goToNext() override;
        bool equals(Iterator *right) override;
        
        ~SetIterator();

        friend class Set;
    };

    Set(MemoryManager &mem): AbstractSet(mem) {};

    int insert(void* elem, size_t size) override;

    Iterator* find(void* elem, size_t size) override;
    Iterator* newIterator() override;
    
    void remove(Iterator *iter) override;
    void clear() override;
    
    inline bool empty() override {return _empty(); };
    inline int size() override { return _size(); };
    inline size_t max_bytes() override { return _get_max_bytes(); };
    
    ~Set();

    friend class SetTester;
};
