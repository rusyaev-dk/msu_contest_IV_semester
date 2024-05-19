#pragma once
#include "Container.h"

// Базовый класс для некоторой группы абстрактных контейнеров
class GroupContainer: public Container
{
protected:
    int _data_array_size = 5000;
    static const int _rehashing_treshhold = 50;
public:
    GroupContainer(MemoryManager &mem): Container(mem) {}

    size_t hash_function(void* key, size_t keySize);
};
