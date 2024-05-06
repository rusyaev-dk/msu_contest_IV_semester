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

    for (size_t i = this->_set_data_index + 1; i < this->_set->_container_size; i++) {
        bool hasNonEmptyList = this->_set->_set_data[i] != nullptr && !this->_set->_set_data[i]->empty(); 
        if (hasNonEmptyList) return true;
    }
    return false;
}

void Set::SetIterator::goToNext() {
    if (!this->_list_iterator->hasNext()) {
        for (size_t i = this->_set_data_index + 1; i < this->_set->_container_size; i++) {
            bool hasNonEmptyList = this->_set->_set_data[i] != nullptr && !this->_set->_set_data[i]->empty();
            if (hasNonEmptyList) {
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
    this->_set_bytes_size = sizeof(LinkedList*) * this->_container_size;
    this->_set_data = (LinkedList**)this->_memory.allocMem(this->_set_bytes_size);
    this->_container_size = this->_container_size;
    this->_elem_count = 0;

    for (size_t i = 0; i < this->_container_size; i++) {
        this->_set_data[i] = nullptr;
    }
}

int Set::insert(void *elem, size_t size) {
    size_t hash = this->hash_function(elem, size);
    if (hash >= this->_container_size) return 2; // скорее всего убрать

    if (this->_set_data[hash] == nullptr) {
        this->_set_data[hash] = new LinkedList(this->_memory);
        int push_err = this->_set_data[hash]->push_front(elem, size);
        if (push_err != 0) {
            this->_memory.freeMem(this->_set_data[hash]);
            this->_set_data[hash] = nullptr;
            return 2;
        }
        this->_elem_count += 1;
        return 0;
    }

    LinkedList::Iterator* list_iter = this->_set_data[hash]->find(elem, size);
    if (list_iter) { // This element already exists
        free(list_iter);
        return 1;
    }
    
    if (this->_set_data[hash]->size() >= this->_rehashing_treshhold) {
        this->_rehash_set();
    }

    int push_err = this->_set_data[hash]->push_front(elem, size);
    if (push_err != 0) {
        free(list_iter);
        return 2;
    }
    
    this->_elem_count += 1;
    free(list_iter);
    return 0;
}

void Set::_rehash_set() {
    std::cout << "Rehashing set...\n";
    
    size_t prev_container_size = this->_container_size;
    this->_container_size *= 2;
    // обработать исключение при невыделении памяти
    LinkedList** new_set_data = (LinkedList**)this->_memory.allocMem(sizeof(LinkedList*) * this->_container_size);

    for (size_t i = 0; i < this->_container_size; i++) {
        new_set_data[i] = nullptr;
    }

    void* elem;
    size_t elem_size;
    size_t new_hash;

    for (size_t i = 0; i < prev_container_size; i++) {
        if (this->_set_data[i] == nullptr) continue;
        LinkedList::Iterator* list_iter = this->_set_data[i]->newIterator();
        
        do {
            elem = list_iter->getElement(elem_size);
            new_hash = this->hash_function(elem, elem_size);
            if (new_set_data[new_hash] == nullptr) {
                // обработать исключение при невыделении памяти
                new_set_data[new_hash] = new LinkedList(this->_memory);
            }
            new_set_data[new_hash]->push_front(elem, elem_size);
            list_iter->goToNext();
        } while (list_iter->hasNext());

        this->_set_data[i]->~LinkedList();
        free(list_iter);
    }
    this->_memory.freeMem(this->_set_data);
    this->_set_data = new_set_data;
}

int Set::size() {
    return this->_elem_count;
}

size_t Set::max_bytes() {
    return this->_set_bytes_size;
}

Set::Iterator* Set::find(void *elem, size_t size) {
    if (this->empty()) return nullptr;

    size_t hash = this->hash_function(elem, size);
    if (this->_set_data[hash] == nullptr) return nullptr;

    LinkedList::Iterator* list_iter = this->_set_data[hash]->find(elem, size);
    if (!list_iter) return nullptr;

    size_t found_elem_size;
    void* found_elem = list_iter->getElement(found_elem_size);
    bool elementsMatch = found_elem_size == size && memcmp(found_elem, elem, size) == 0;
    
    if (elementsMatch) {
        return new SetIterator(list_iter, this, hash);
    }
    
    free(list_iter);
    return nullptr;
}

Set::Iterator* Set::newIterator() {
    if (this->empty()) return nullptr;
    
    for (size_t i = 0; i < this->_container_size; i++) {
        if (this->_set_data[i] != nullptr) {
            return new SetIterator(_set_data[i]->newIterator(), this, i);
        }
    }
    return nullptr;
}

Set::Iterator* Set::_newIterator(size_t set_data_index) {
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

    bool isLastElemInList = !casted_iter->_list_iterator->hasNext();
    if (isLastElemInList) {
        iter->goToNext();
    }

    size_t index = casted_iter->_set_data_index;
    this->_set_data[index]->remove(duplicated_iter->_list_iterator);    
    this->_elem_count -= 1;

    free(duplicated_iter);
}

void Set::clear() {
    if (this->empty()) return;

    for (size_t i = 0; i < this->_container_size; i++) {
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
    for (size_t i = 0; i < this->_container_size; i++) {
        if (this->_set_data[i] != nullptr) {
            this->_memory.freeMem(this->_set_data[i]);
            this->_set_data[i] = nullptr;
        }
    }

    this->_memory.freeMem(this->_set_data);
    std::cout << "Set destroyed" << "\n";
}
