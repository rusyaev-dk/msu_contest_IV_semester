#include "Set.h"
#include "LinkedList1.h"

Set::SetIterator::SetIterator(Iterator* iterator, Set* set, int set_data_index) : set(set), set_data_index(set_data_index) {
    this->list_iterator = dynamic_cast<LinkedList::ListIterator*>(iterator);
}

void* Set::SetIterator::getElement(size_t &size) {
    return this->list_iterator->getElement(size);
}

bool Set::SetIterator::hasNext() {
    if (this->list_iterator->hasNext()) return true;

    for (int i = this->set_data_index; i < this->set->set_data_len; i++) {
        if (this->set->set_data[i] != nullptr) return true;
    }
    return false;
}

void Set::SetIterator::goToNext() {
    if (!this->list_iterator->hasNext()) {
        for (int i = this->set_data_index; i < this->set->set_data_len; i++) {
            if (this->set->set_data[i] != nullptr) {
                this->list_iterator = dynamic_cast<LinkedList::ListIterator*>(this->set->set_data[i]->newIterator());
                this->set_data_index = i;
                return;
            }
        }
        return;
    }
    this->list_iterator->goToNext();
}

bool Set::SetIterator::equals(Iterator* right) {
    Set::SetIterator* casted_right = dynamic_cast<Set::SetIterator*>(right);
    return this->list_iterator->equals(casted_right->list_iterator);
}

Set::Set(MemoryManager &mem) : AbstractSet(mem) {
    this->set_size = sizeof(LinkedList*) * 10000;
    this->set_data = (LinkedList**)this->_memory.allocMem(this->set_size);
    this->set_data_len = 10000;
    this->elem_count = 0;

    for (int i = 0; i < 10000; i++) {
        this->set_data[i] = nullptr;
    }
}

int Set::insert(void *elem, size_t size) {
    size_t elem_index = this->hash_function(elem, size);
    if (elem_index >= this->set_size) return -1;
    
    if (!this->set_data[elem_index]) {
        this->set_data[elem_index] = new LinkedList(this->_memory);
    }
    this->set_data[elem_index]->push_front(elem, size);
    this->elem_count += 1;
    return 0;
}

int Set::size() {
    return this->elem_count;
}

size_t Set::max_bytes() {
    return this->set_size;
}

Set::Iterator* Set::find(void *elem, size_t size) {
    size_t elem_index = this->hash_function(elem, size);
    Set::SetIterator* set_iterator = new SetIterator(set_data[elem_index]->newIterator(), this, elem_index);
    return set_data[elem_index]->find(elem, size);
}

Set::Iterator* Set::newIterator() {
    if (this->elem_count == 0) return nullptr;
    
    for (int i = 0; i < this->set_data_len; i++) {
        if (this->set_data[i] != nullptr) return new SetIterator(set_data[i]->newIterator(), this, i);
    }
    return nullptr;
}

void Set::remove(Iterator* iter) {
    Set::SetIterator* casted_iter = dynamic_cast<Set::SetIterator*>(iter);
    
    size_t elem_size;
    void* elem = casted_iter->getElement(elem_size);
    size_t elem_index = this->hash_function(elem, elem_size);

    Set::SetIterator* set_iterator = new SetIterator(set_data[elem_index]->newIterator(), this, elem_index);
    
    for (int i = elem_index; i < this->set_data_len; i++) {
        if (set_iterator->equals(casted_iter)) {
            // deleting...
            // set_iterator->list_iterator->l->pop_back();
            this->elem_count -= 1;
            free(set_iterator);
            return;
        }
        set_iterator->goToNext();
    }
    free(set_iterator);
}

void Set::clear() {
    if (this->elem_count == 0) return;
    Set::SetIterator* set_iterator = dynamic_cast<Set::SetIterator*>(this->newIterator());
    
    for (int i = set_iterator->set_data_index; i < this->set_data_len; i++) {
        for (int j = 0; j < set_iterator->list_iterator->l->size(); j++) {
            set_iterator->list_iterator->l->pop_front();
        }
        if (!set_iterator->hasNext()) break;
        set_iterator->goToNext();
    }
    free(set_iterator);
}

bool Set::empty() {
    return (this->elem_count == 0);
}

Set::~Set() {
    // this->_memory.freeMem(this->set_data); 
}

