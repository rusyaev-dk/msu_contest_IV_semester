#include "GroupContainer.h"

#include <iostream>

using namespace std;
using IteratorUtils = GroupContainer::IteratorUtils;
using ListIterator = LinkedList1::ListIterator;

static const size_t _FNV_PRIME = 2^40 + 2^8 + 0xB3;
static const size_t _OFFSET_BASIS = 0x811C9DC5;

GroupContainer::GroupContainer(MemoryManager &mem, ContainerType type): Container(mem) {
    this->_type = type;
    this->_max_bytes = sizeof(LinkedList1*) * this->_data_array_size;
    this->_data_array = (LinkedList1**)this->_memory.allocMem(this->_max_bytes);
    if (!this->_data_array) {
        throw Error("Memory allocation error in set constructor.");
    }

    for (size_t i = 0; i < this->_data_array_size; i++) {
        this->_data_array[i] = nullptr;
    }

    this->_elem_count = 0;
}

size_t GroupContainer::hash_function(void* key, size_t keySize) {
    // https://www.ietf.org/id/draft-eastlake-fnv-22.html

    unsigned char* data = (unsigned char*)key;

    size_t hash = _OFFSET_BASIS;
    for (size_t i = 0; i < keySize; i++) {
        hash ^= (size_t)data[i];
        hash *= _FNV_PRIME;
    }
    return hash % this->_data_array_size;
}

// size_t IteratorUtils::get_elem_hash(GroupContainer* container, Iterator* list_iter) {
//     size_t elem_size;
//     void* elem = list_iter->getElement(elem_size);
//     if (!elem) {
//         throw Error("Elem in active iterator is null.");
//     }
//     return container->hash_function(elem, elem_size);
// }

void* IteratorUtils::getElement(size_t& size, GroupContainer* container, Iterator* list_iter) {
    if (container->empty() || size == 0) return nullptr;
    return list_iter->getElement(size);
}

bool IteratorUtils::hasNext(GroupContainer* container, Iterator* list_iter) {
    if (container->empty()) return false;
    if (list_iter->hasNext()) return true;

    for (size_t i = get_elem_hash(container, list_iter) + 1; i < container->_data_array_size; i++) {
        bool has_non_empty_list = container->_data_array[i] != nullptr && !container->_data_array[i]->empty(); 
        if (has_non_empty_list) return true;
    }
    return false;
}

void IteratorUtils::goToNext(GroupContainer* container, ListIterator*& list_iter) {
    if (container->empty()) return;
    if (list_iter->hasNext()) {
        list_iter->goToNext();
        return;
    }
    
    for (size_t i = get_elem_hash(container, list_iter) + 1; i < container->_data_array_size; i++) {
        bool has_non_empty_list = container->_data_array[i] != nullptr && !container->_data_array[i]->empty();
        if (has_non_empty_list) {
            container->_memory.freeMem(list_iter);
            ListIterator* new_list_iter = static_cast<ListIterator*>(container->_data_array[i]->newIterator());
            list_iter = new_list_iter;
            return;
        }
    }
}
