#include "Set.h"
#include "LinkedList1.h"

#include <iostream>

using namespace std;
using ListIterator = LinkedList1::ListIterator;
using SetIterator = Set::SetIterator;

static const int _REHASHING_THRESHOLD = 50;

SetIterator::SetIterator(Iterator* iterator, Set* set) : _set(set) {
    this->_list_iter = static_cast<ListIterator*>(iterator);
}

SetIterator::~SetIterator() { this->_set->_memory.freeMem(this->_list_iter); }

int Set::insert(void *elem, size_t size) {
    if (!elem || size == 0) return 2;
    
    size_t hash = this->hash_function(elem, size);
    if (this->_data_array[hash] == nullptr) {
        this->_data_array[hash] = new LinkedList1(this->_memory);
        if (!this->_data_array[hash]) {
            throw Error("Memory allocation error for linked list in insert function.");        
        }

        int push_err = this->_data_array[hash]->push_front(elem, size);
        if (push_err != 0) return 2;
        
        this->_elem_count += 1;
        return 0;
    }

    LinkedList1::Iterator* list_iter = this->_data_array[hash]->find(elem, size);
    if (list_iter) { // Element already exists
        this->_memory.freeMem(list_iter);
        return 1;
    }
    
    bool needs_rehash = this->_data_array[hash]->size() >= _REHASHING_THRESHOLD; 
    if (needs_rehash) {
        cout << "Rehashing set\n";
        this->_rehash_set();
        hash = this->hash_function(elem, size);
    }

    int push_err = this->_data_array[hash]->push_front(elem, size);
    if (push_err != 0) return 2;

    this->_elem_count += 1;
    return 0;
}

void Set::_rehash_set() {
    this->_data_array_size *= 2; // Increases here because of hash_function computing

    size_t new_max_bytes = sizeof(LinkedList1*) * this->_data_array_size;
    LinkedList1** new_data_array = (LinkedList1**)this->_memory.allocMem(new_max_bytes);
    if (!new_data_array) {
        throw Error("Memory allocation error for new_data_array in set rehashing.");
    }

    for (size_t i = 0; i < this->_data_array_size; i++) {
        new_data_array[i] = nullptr;
    }

    for (size_t i = 0; i < this->_data_array_size / 2; i++) {
        if (this->_data_array[i] == nullptr) continue;

        LinkedList1::Iterator* list_iter = this->_data_array[i]->newIterator();
        int list_size = this->_data_array[i]->size();

        try {
            for (size_t j = 0; j < list_size; j++) {
                this->_rehash_single_elem(new_data_array, list_iter);
            }
        } catch (...) {
            this->_rehash_err_cleanup(new_data_array, list_iter);
            throw;
        }

        this->_memory.freeMem(list_iter);
        this->_data_array[i]->clear();
        this->_memory.freeMem(this->_data_array[i]);
    }
    this->_memory.freeMem(this->_data_array);

    this->_data_array = new_data_array;
    this->_max_bytes = new_max_bytes;
}

void Set::_rehash_single_elem(LinkedList1** new_data_array, Iterator* list_iter) {
    size_t elem_size;
    void* elem = list_iter->getElement(elem_size);
    size_t new_hash = this->hash_function(elem, elem_size);

    if (new_data_array[new_hash] == nullptr) {
        new_data_array[new_hash] = new LinkedList1(this->_memory);
        if (!new_data_array[new_hash]) {
            throw Error("Memory allocation error for linked list in set rehashing.");
        }
    }

    int push_err = new_data_array[new_hash]->push_front(elem, elem_size);
    if (push_err != 0) {
        throw Error("Couldn't push the rehashed element.");
    }

    list_iter->goToNext();
}

void Set::_rehash_err_cleanup(LinkedList1** new_data_array, Iterator* list_iter) {
    for (size_t i = 0; i < this->_data_array_size; i++) {
        if (new_data_array[i] == nullptr) continue;

        new_data_array[i]->clear();
        this->_memory.freeMem(new_data_array[i]);
    }

    this->_memory.freeMem(list_iter);
    this->_memory.freeMem(new_data_array);
    this->_data_array_size /= 2;
}

Set::Iterator* Set::find(void* elem, size_t size) {
    if (this->empty() || !elem || size == 0) return nullptr;

    size_t hash = this->hash_function(elem, size);
    if (this->_data_array[hash] == nullptr) return nullptr;

    LinkedList1::Iterator* list_iter = this->_data_array[hash]->find(elem, size);
    if (!list_iter) return nullptr;

    size_t found_elem_size;
    void* found_elem = list_iter->getElement(found_elem_size);
    bool elems_not_matched = !(found_elem_size == size && memcmp(found_elem, elem, size) == 0);
    if (elems_not_matched) {
        this->_memory.freeMem(list_iter);
        return nullptr;
    }

    return new SetIterator(list_iter, this);
}

Set::Iterator* Set::newIterator() {
    if (this->empty()) return nullptr;
    
    for (size_t i = 0; i < this->_data_array_size; i++) {
        bool has_non_empty_list = this->_data_array[i] != nullptr && !this->_data_array[i]->empty();
        if (has_non_empty_list) {
            return new SetIterator(this->_data_array[i]->newIterator(), this);
        }
    }
    return nullptr;
}

void Set::remove(Iterator* iter) {
    if (this->empty() || !iter) return;

    SetIterator* casted_iter = static_cast<SetIterator*>(iter);
    if (casted_iter->_set != this) return;
    size_t hash = casted_iter->_get_elem_hash();

    bool reached_list_end = !casted_iter->_list_iter->hasNext();
    if (reached_list_end) {
        ListIterator list_iter_to_remove = *casted_iter->_list_iter;
        iter->goToNext();
        this->_data_array[hash]->remove(&list_iter_to_remove);
    } else {
        this->_data_array[hash]->remove(casted_iter->_list_iter);
    }

    this->_elem_count -= 1;
}

void Set::clear() {
    if (this->empty()) return;

    for (size_t i = 0; i < this->_data_array_size; i++) {
        bool has_non_empty_list = this->_data_array[i] != nullptr && !this->_data_array[i]->empty(); 
        if (has_non_empty_list) {
            this->_data_array[i]->clear();
        }
    }
    
    this->_elem_count = 0;
    cout << "Set cleared\n";
}

Set::~Set() {
    this->clear();
    for (size_t i = 0; i < this->_data_array_size; i++) {
        if (this->_data_array[i] != nullptr) {
            this->_memory.freeMem(this->_data_array[i]);
        }
    }

    this->_memory.freeMem(this->_data_array);
    cout << "Set destroyed\n";
}
