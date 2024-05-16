#include "Set.h"
#include "LinkedList1.h"

#include <iostream>

Set::SetIterator::SetIterator(Iterator* iterator, Set* set) : _set(set) {
    this->_list_iterator = dynamic_cast<LinkedList1::ListIterator*>(iterator);
}

Set::SetIterator::~SetIterator() { delete this->_list_iterator; }

size_t Set::SetIterator::_get_data_arr_index() {
    size_t elem_size;
    void* elem = this->getElement(elem_size);
    if (!elem) {
        throw Error("Elem in set iterator is null.");
    }
    return this->_set->hash_function(elem, elem_size);
}

void* Set::SetIterator::getElement(size_t& size) {
    return this->_list_iterator->getElement(size);
}

bool Set::SetIterator::hasNext() {
    if (this->_list_iterator->hasNext()) return true;

    for (size_t i = this->_get_data_arr_index() + 1; i < this->_set->_data_array_size; i++) {
        bool has_non_empty_list = this->_set->_data_array[i] != nullptr && !this->_set->_data_array[i]->empty(); 
        if (has_non_empty_list) return true;
    }
    return false;
}

void Set::SetIterator::goToNext() {
    if (!this->_list_iterator->hasNext()) {
        for (size_t i = this->_get_data_arr_index() + 1; i < this->_set->_data_array_size; i++) {
            bool has_non_empty_list = this->_set->_data_array[i] != nullptr && !this->_set->_data_array[i]->empty();
            if (has_non_empty_list) {
                this->_list_iterator = dynamic_cast<LinkedList1::ListIterator*>(this->_set->_data_array[i]->newIterator());
                return;
            }
        }
        return;
    }
    this->_list_iterator->goToNext();
}

bool Set::SetIterator::equals(Iterator* right) {
    Set::SetIterator* casted_right = dynamic_cast<Set::SetIterator*>(right);
    return this->_list_iterator->equals(casted_right->_list_iterator);
}

Set::Set(MemoryManager& mem) : AbstractSet(mem) {
    this->_bytes_size = sizeof(LinkedList1*) * this->_data_array_size;
    this->_data_array = (LinkedList1**)this->_memory.allocMem(this->_bytes_size);
    if (!this->_data_array) {
        throw Error("Memory allocation error in set constructor.");
    }

    for (size_t i = 0; i < this->_data_array_size; i++) {
        this->_data_array[i] = nullptr;
    }

    this->_elem_count = 0;
}

int Set::insert(void *elem, size_t size) {
    if (!elem || size <= 0) return 2;
    
    size_t hash = this->hash_function(elem, size);
    if (this->_data_array[hash] == nullptr) {
        this->_data_array[hash] = new LinkedList1(this->_memory);
        if (!this->_data_array[hash]) {
            throw Error("Memory allocation error. Tried to create linked list in insert function.");        
        }

        int push_err = this->_data_array[hash]->push_front(elem, size);
        if (push_err != 0) return 2;
        
        this->_elem_count += 1;
        return 0;
    }

    LinkedList1::Iterator* list_iter = this->_data_array[hash]->find(elem, size);
    if (list_iter) { // Element already exists
        delete list_iter;
        return 1;
    }
    delete list_iter;
    
    bool needs_rehash = this->_data_array[hash]->size() >= this->_rehashing_treshhold; 
    if (needs_rehash) {
        std::cout << "Rehashing set...\n";
        this->_rehash_set();
        hash = this->hash_function(elem, size);
    }

    int push_err = this->_data_array[hash]->push_front(elem, size);
    if (push_err != 0) return 2;

    this->_elem_count += 1;
    return 0;
}

void Set::_rehash_set() {
    size_t prev_data_arr_size = this->_data_array_size;

    size_t new_bytes_size = sizeof(LinkedList1*) * prev_data_arr_size * 2;
    LinkedList1** new_data_array = (LinkedList1**)this->_memory.allocMem(new_bytes_size);
    if (!new_data_array) {
        throw Error("Memory allocation error for new_data_array in set rehashing.");
    }

    this->_data_array_size *= 2;

    for (size_t i = 0; i < this->_data_array_size; i++) {
        new_data_array[i] = nullptr;
    }

    for (size_t i = 0; i < prev_data_arr_size; i++) {
        if (this->_data_array[i] == nullptr) continue;
        LinkedList1::Iterator* list_iter = this->_data_array[i]->newIterator();
        
        for (size_t j = 0; j < this->_data_array[i]->size(); j++) {
            size_t elem_size;
            void* elem = list_iter->getElement(elem_size);

            size_t new_hash = this->hash_function(elem, elem_size);
            if (new_data_array[new_hash] == nullptr) {
                new_data_array[new_hash] = new LinkedList1(this->_memory);
                if (!new_data_array[new_hash]) {
                    free(new_data_array);
                    delete list_iter;
                    this->_data_array_size = prev_data_arr_size;
                    throw Error("Memory allocation error for linked list in set rehashing.");
                }
            }

            int push_err = new_data_array[new_hash]->push_front(elem, elem_size);
            if (push_err != 0) {
                free(new_data_array);
                delete list_iter;
                this->_data_array_size = prev_data_arr_size;
                throw Error("Couldn't push the rehashed element.");
            }

            list_iter->goToNext();
        }
        delete list_iter;
        delete this->_data_array[i];
    }
    this->_memory.freeMem(this->_data_array);

    this->_bytes_size = new_bytes_size;
    this->_data_array = new_data_array;
}

Set::Iterator* Set::find(void *elem, size_t size) {
    if (this->empty() || !elem) return nullptr;

    size_t hash = this->hash_function(elem, size);
    if (this->_data_array[hash] == nullptr) return nullptr;

    LinkedList1::Iterator* list_iter = this->_data_array[hash]->find(elem, size);
    if (!list_iter) return nullptr;

    size_t found_elem_size;
    void* found_elem = list_iter->getElement(found_elem_size);
    bool elems_not_matched = !(found_elem_size == size && memcmp(found_elem, elem, size) == 0);
    if (elems_not_matched) {
        delete list_iter;
        return nullptr;
    }

    return new SetIterator(list_iter, this);
}

Set::Iterator* Set::newIterator() {
    if (this->empty()) return nullptr;
    
    for (size_t i = 0; i < this->_data_array_size; i++) {
        if (this->_data_array[i] != nullptr) {
            return new SetIterator(this->_data_array[i]->newIterator(), this);
        }
    }
    return nullptr;
}

void Set::remove(Iterator* iter) {
    if (this->empty()) return;

    Set::SetIterator* casted_iter = dynamic_cast<Set::SetIterator*>(iter);
    if (casted_iter->_set != this) return;

    size_t index = casted_iter->_get_data_arr_index();
    bool reached_list_end = !casted_iter->_list_iterator->hasNext();
    
    if (reached_list_end) {
        LinkedList1::ListIterator list_iter_to_remove = *casted_iter->_list_iterator;
        iter->goToNext();
        this->_data_array[index]->remove(&list_iter_to_remove);
    } else {
        this->_data_array[index]->remove(casted_iter->_list_iterator);
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
    std::cout << "Set cleared\n";
}

bool Set::empty() { return this->_elem_count == 0; }

int Set::size() { return this->_elem_count; }

size_t Set::max_bytes() { return this->_bytes_size; }

Set::~Set() {
    this->clear();
    for (size_t i = 0; i < this->_data_array_size; i++) {
        if (this->_data_array[i] != nullptr) {
            delete this->_data_array[i];
        }
    }

    this->_memory.freeMem(this->_data_array);
    std::cout << "Set destroyed\n";
}
