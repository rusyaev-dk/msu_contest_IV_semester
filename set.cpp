#include "Set.h"

static void* set_data;

Set::Set(MemoryManager &mem) : AbstractSet(mem) {
    this->_memory.allocMem(100);

}

int Set::insert(void *elem, size_t size) {
    
}

Set::~Set() {
    // this->_memory.freeMem(); 
}


