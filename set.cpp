#include "Set.h"
#include "LinkedList1.h"
#include <iostream>

Set::SetIterator::SetIterator(Iterator* iterator, Set* set, size_t set_data_index) : _set(set), _set_data_index(set_data_index) {
    this->_list_iterator = dynamic_cast<LinkedList::ListIterator*>(iterator);
}

void* Set::SetIterator::getElement(size_t& size) {
    return this->_list_iterator->getElement(size);
}

bool Set::SetIterator::hasNext() {
    if (this->_list_iterator->hasNext()) return true;

    for (size_t i = this->_set_data_index + 1; i < this->_set->_set_data_len; i++) {
        if (this->_set->_set_data[i] != nullptr && !this->_set->_set_data[i]->empty()) return true;
    }
    return false;
}

void Set::SetIterator::goToNext() {
    if (!this->_list_iterator->hasNext()) {
        for (size_t i = this->_set_data_index + 1; i < this->_set->_set_data_len; i++) {
            if (this->_set->_set_data[i] != nullptr && !this->_set->_set_data[i]->empty()) {
                // free(this->list_iterator);
                this->_list_iterator = dynamic_cast<LinkedList::ListIterator*>(this->_set->_set_data[i]->newIterator());
                this->_set_data_index = i;
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

Set::Set(MemoryManager &mem) : AbstractSet(mem) {
    this->_set_bytes_size = sizeof(LinkedList*) * this->container_size;
    this->_set_data = (LinkedList**)this->_memory.allocMem(this->_set_bytes_size);
    this->_set_data_len = this->container_size;
    this->_elem_count = 0;

    for (size_t i = 0; i < this->container_size; i++) {
        this->_set_data[i] = nullptr;
    }
}

int Set::insert(void *elem, size_t size) {
    size_t hash = this->hash_function(elem, size);
    if (hash >= this->_set_data_len) return 2;

    if (this->_set_data[hash] == nullptr) {
        this->_set_data[hash] = new LinkedList(this->_memory);
        int err = this->_set_data[hash]->push_front(elem, size);
        if (err != 0) {
       
            this->_memory.freeMem(this->_set_data[hash]);
            this->_set_data[hash] = nullptr;
            return 2;
        }
        this->_elem_count += 1;
        return 0;
    }
    LinkedList::ListIterator* list_iter = dynamic_cast<LinkedList::ListIterator*>(this->_set_data[hash]->find(elem, size));
    if (list_iter != nullptr) return 1;
    
    int err = this->_set_data[hash]->push_front(elem, size);
    if (err != 0) return 2;
    
    this->_elem_count += 1;
    return 0;
}

int Set::size() {
    return this->_elem_count;
}

size_t Set::max_bytes() {
    return this->_set_bytes_size;
}

Set::Iterator* Set::find(void *elem, size_t size) {
    if (this->empty()) return nullptr;

    size_t set_data_hash = this->hash_function(elem, size);
    if (_set_data[set_data_hash] == nullptr) return nullptr;

    LinkedList::ListIterator* list_iterator = dynamic_cast<LinkedList::ListIterator*>(_set_data[set_data_hash]->find(elem, size));
    if (list_iterator == nullptr) return nullptr;

    size_t found_elem_size;
    void* found_elem = list_iterator->getElement(found_elem_size);

    if (found_elem_size == size && memcmp(found_elem, elem, size) == 0) {
        return new SetIterator(list_iterator, this, set_data_hash);
    }
    
    return nullptr;
}

Set::Iterator* Set::newIterator() {
    if (this->empty()) return nullptr;
    
    for (size_t i = 0; i < this->_set_data_len; i++) {
        if (this->_set_data[i] != nullptr) return new SetIterator(_set_data[i]->newIterator(), this, i);
    }
    return nullptr;
}

Set::Iterator* Set::newIterator(size_t set_data_index) {
    if (this->empty()) return nullptr;
    return new SetIterator(_set_data[set_data_index]->newIterator(), this, set_data_index);
}

void Set::remove(Iterator* iter) {
    if (this->empty()) return;
    Set::SetIterator* casted_iter = dynamic_cast<Set::SetIterator*>(iter);
    if (casted_iter->_set != this) return;
    Set::SetIterator* duplicated_iter = new SetIterator(casted_iter->_list_iterator, this, casted_iter->_set_data_index);

    // size_t elem_size;
    // void* elem = casted_iter->getElement(elem_size);
    // _________________________________
    // size_t found_elem_size;
    // double found_elem = *(double*)set_iterator->getElement(found_elem_size);
    // _________________________________

    if (!casted_iter->_list_iterator->hasNext()) {
        iter->goToNext();
    }

    size_t index = casted_iter->_set_data_index;
    this->_set_data[index]->remove(duplicated_iter->_list_iterator);    
    this->_elem_count -= 1;

    free(duplicated_iter);
}

void Set::clear() {
    if (this->empty()) return;

    for (size_t i = 0; i < this->_set_data_len; i++) {
        if (this->_set_data[i] != nullptr) {
            this->_set_data[i]->clear();
        }
    }
    
    this->_elem_count = 0;
    std::cout << "Set cleared." << "\n";
}

bool Set::empty() {
    return (this->_elem_count == 0);
}

Set::~Set() {
    this->clear();
    for (size_t i = 0; i < this->_set_data_len; i++) {
        if (this->_set_data[i] != nullptr) {
            this->_memory.freeMem(this->_set_data[i]);
            this->_set_data[i] = nullptr;
        }
    }

    this->_memory.freeMem(this->_set_data);
    std::cout << "Set destroyed" << "\n";
}
