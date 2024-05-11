#include "GroupContainer.h"
#include <iostream>

// https://www.ietf.org/id/draft-eastlake-fnv-22.html

static const size_t _FNV_PRIME = 2^40 + 2^8 + 0xB3;
static const size_t _OFFSET_BASIS = 0x811C9DC5;

size_t GroupContainer::hash_function(void* key, size_t keySize) {
    unsigned char* data = (unsigned char*)key;

    size_t hash = _OFFSET_BASIS;
    for (size_t i = 0; i < keySize; i++) {
        hash ^= (size_t)data[i];
        hash *= _FNV_PRIME;
    }
    return hash % this->_container_size;
}
