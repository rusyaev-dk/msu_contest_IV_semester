#pragma once
#include "MainMemoryManager.h"
#include "Set.h"

class SetTester {
private:
    Set* _set;
    MainMemoryManager* _mem_manager;

public:
    SetTester(size_t mem_size);

    int insert_test(int test_elem_count);

    ~SetTester();
};