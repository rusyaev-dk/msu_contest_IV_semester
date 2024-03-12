#pragma once
#include "MemoryManager.h"

class MainMemoryManager : public MemoryManager {
private:
    void* mem_pointer;
public:
    MainMemoryManager(size_t size) : MemoryManager(size) {}

    void* allocMem(size_t size)
    {
        return new char[size];
    }

    void freeMem(void* ptr)
    {
        delete[] ptr;
    }
};