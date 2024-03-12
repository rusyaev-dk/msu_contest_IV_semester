#include "Set.h"
#include "LinkedList1.h"

Set::SetIterator::SetIterator(Iterator* iterator, Set* set, int set_data_index) : set(set), set_data_index(set_data_index) {
    this->list_iterator = dynamic_cast<LinkedList::ListIterator*>(iterator);
}

void* Set::SetIterator::getElement(size_t &size) {
    return this->list_iterator->getElement(size);
}

bool Set::SetIterator::hasNext() {
    if (!this->list_iterator->hasNext()) {
        this->goToNext();
        return this->hasNext();
    }
    return true;
}

void Set::SetIterator::goToNext() {
    if (!this->list_iterator->hasNext()) {
        int i = 0;
        while (this->set->set_data[i] != nullptr) {
            i++;
        }
        if (this->set->set_data[i] != nullptr) {
            this->list_iterator = dynamic_cast<LinkedList::ListIterator*>(this->set->set_data[i]->newIterator());
        }
        return;
    }
    this->list_iterator->goToNext();
}

bool Set::SetIterator::equals(Iterator* right) {
    LinkedList::ListIterator* castedRight = dynamic_cast<LinkedList::ListIterator*>(right);
    return this->list_iterator->equals(castedRight);
}

Set::Set(MemoryManager &mem) : AbstractSet(mem) {
    this->set_size = 100000;
    this->set_data = (LinkedList**)this->_memory.allocMem(sizeof(LinkedList*) * this->set_size);
    
    for (int i = 0; i < this->set_size; i++) {
        this->set_data[i] = nullptr;
    }
}

int Set::insert(void *elem, size_t size) {
    size_t hash = this->hash_function(elem, size);
    if (hash >= this->set_size) {
        return -1;
    }
    if (!this->set_data[hash]) {
        this->set_data[hash] = new LinkedList(this->_memory);
    }
    this->set_data[hash]->push_front(elem, size);
    return 0;
}

int Set::size() {
    int i = 0, result = 0;
    for (int i = 0; i < this->set_size; i++) {
        if (this->set_data[i] != nullptr) {
            result += this->set_data[i]->size();
        }
    }
    return result;
}

Set::Iterator* Set::newIterator() {
    int i = 0;
    while (this->set_data[i] == nullptr) {
        i++;
    }
    // std::cout << i ;
    return new SetIterator(set_data[i]->newIterator(), this, i);
}

Set::~Set() {
    // this->_memory.freeMem(this->set_data); 
}

