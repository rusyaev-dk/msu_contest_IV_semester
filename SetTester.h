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

    void insert_test(size_t insert_elem_count);
    void find_test(size_t insert_elem_count);
    void remove_even_test(size_t insert_elem_count);
    void duplicate_iterator_test();

    friend class SetTesterException;

    ~SetTester();
};
