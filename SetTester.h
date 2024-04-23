#pragma once
#include "MainMemoryManager.h"
#include "SetTesterExceptions.h"
#include "Set.h"

class SetTester {
private:
    Set* _set;
    MainMemoryManager* _mem_manager;

public:
    SetTester(size_t mem_bytes_size);

    void insert_test();
    void find_test();
    void remove_even_test();
    void duplicate_iterator_test();

    friend class SetTesterException;

    ~SetTester();
};
