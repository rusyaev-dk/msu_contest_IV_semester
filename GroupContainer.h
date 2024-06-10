#pragma once
#include "Container.h"
#include "LinkedList1.h"

// Базовый класс для некоторой группы абстрактных контейнеров
class GroupContainer: public Container
{
protected:
    LinkedList1** _data_array;
    
    size_t _data_array_size = 5000;

    // actual quantity of elems in container
    size_t _elem_count;

    // max container size in bytes
    size_t _max_bytes;

    size_t _iter_get_elem_hash(Iterator* list_iter);
    void* _iterGetElement(size_t& size, Iterator* list_iter);
    bool _iterHasNext(Iterator* list_iter);
    // void _iterGoToNext(LinkedList1::ListIterator* list_iter);

    inline bool _empty() { return _elem_count == 0; };
    inline int _size() { return this->_elem_count; };
    inline size_t _get_max_bytes() { return this->_max_bytes; }; 

public:
    GroupContainer(MemoryManager &mem);

    size_t hash_function(void* key, size_t keySize);
};
