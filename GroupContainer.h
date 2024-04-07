#pragma once
#include "Container.h"

// Базовый класс для некоторой группы абстрактных контейнеров
class GroupContainer: public Container
{
protected:
    const int arr_size= 6;

public:
    GroupContainer(MemoryManager &mem): Container(mem) {}
    //size_t hash_function(void* key, size_t keySize);
};
