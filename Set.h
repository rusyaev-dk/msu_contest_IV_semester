#pragma once

#include "SetAbstract.h"

using namespace std;
using IteratorUtils = GroupContainer::IteratorUtils;

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
        size_t _get_elem_hash() { return IteratorUtils::get_elem_hash(_set, _list_iter); };
        
    public:
        void* getElement(size_t &size) override { return IteratorUtils::getElement(size, _set, _list_iter); };
        bool hasNext() override { return IteratorUtils::hasNext(_set, _list_iter); };
        void goToNext() override { IteratorUtils::goToNext(_set, _list_iter); };
        bool equals(Iterator *right) override { return IteratorUtils::equals<SetIterator>(this, static_cast<SetIterator*>(right)); };
        
        ~SetIterator();

        friend class GroupContainer;
        friend class IteratorUtils;
        friend class Set;
    };

    Set(MemoryManager &mem): AbstractSet(mem) {};

    int insert(void* elem, size_t size) override;

    Iterator* find(void* elem, size_t size) override;
    Iterator* newIterator() override { return _newIterator<Set, SetIterator>(this); };
    
    void remove(Iterator *iter) override;
    void clear() override;
    
    inline bool empty() override { return _empty(); };
    inline int size() override { return _size(); };
    inline size_t max_bytes() override { return _get_max_bytes(); };
    
    ~Set();

    friend class SetTester;
};
