#pragma once
#include "MemoryManager.h"

class MainMemoryManager : public MemoryManager {
private:
    void* mem_pointer;
public:
    MainMemoryManager(size_t size) : MemoryManager(size) {}

    void* allocMem(size_t size) {
        // try {
            return new char[size];
        // } catch (const std::bad_alloc& e) {
        //     throw this->;
        // }
    }

    void freeMem(void* ptr)
    {
        // delete[] ptr;
    }
};
