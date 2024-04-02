#include "GroupContainer.h"
#include <iostream>

size_t GroupContainer::hash_function(void* key, size_t keySize) {
    unsigned char* data = (unsigned char*)key;
    size_t hash = 0;
    for (size_t i = 0; i < keySize; ++i) {
        hash += data[i];
    }
    hash %= this->container_size;
    std::cout << "hash: " << hash << "\n";
    return hash;

    // unsigned char* data = (unsigned char*)key;
    // size_t hash = 14695981039346656037ULL; // FNV offset basis

    // for (size_t i = 0; i < keySize; ++i) {
    //     hash ^= (size_t)data[i];
    //     hash *= 1099511628211ULL; // FNV prime
    // }

    // hash %= this->container_size;
    // std::cout << "hash: " << hash << "\n";
    // return hash;
}
