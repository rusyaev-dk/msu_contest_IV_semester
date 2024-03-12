#include "GroupContainer.h"
#include <iostream>

size_t GroupContainer::hash_function(void* key, size_t keySize) {
    unsigned char* data = (unsigned char*)key;
    size_t hash = 0;

    for (size_t i = 0; i < keySize; ++i) {
        hash += data[i];
    }

    hash %= 100000;
    std::cout << "hash: " << hash;
    return hash;

}
