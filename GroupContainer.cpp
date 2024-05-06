#include "GroupContainer.h"
#include <iostream>

size_t GroupContainer::hash_function(void* key, size_t keySize) {
    unsigned char* data = (unsigned char*)key;
    const size_t fnv_prime = 1099511628211u;
    size_t hash = 14695981039346656037u;
    for (size_t i = 0; i < keySize; ++i) {
        hash ^= (size_t)data[i];
        hash *= fnv_prime;
    }
    return hash % this->_container_size;
}
