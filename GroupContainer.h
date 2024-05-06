#pragma once
#include "Container.h"

// Базовый класс для некоторой группы абстрактных контейнеров
class GroupContainer: public Container
{
protected:
    int _container_size = 50000;
    int _rehashing_treshhold = 50;
public:
    GroupContainer(MemoryManager &mem): Container(mem) {}

    size_t hash_function(void* key, size_t keySize);
};
