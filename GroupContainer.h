#pragma once
#include "Container.h"
#include "LinkedList1.h"

using namespace std;

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

    inline bool _empty() { return _elem_count == 0; };
    inline int _size() { return this->_elem_count; };
    inline size_t _get_max_bytes() { return this->_max_bytes; }; 

public:
    class IteratorUtils {
    public:
        static size_t get_elem_hash(GroupContainer* container, Iterator* list_iter);
        static void* getElement(size_t& size, GroupContainer* container, Iterator* list_iter);
        static bool hasNext(GroupContainer* container, Iterator* list_iter);
        static void goToNext(GroupContainer* container, LinkedList1::ListIterator*& list_iter);
        
        template <typename T>
        static bool equals(T* left, T* right) {
            if (!left || !right) return false;
            return left->_list_iter->equals(right->_list_iter);
        }
    };

    GroupContainer(MemoryManager &mem);

    size_t hash_function(void* key, size_t keySize);

    friend class IteratorUtils;
};
