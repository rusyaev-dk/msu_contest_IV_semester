#pragma once

#include "MainMemoryManager.h"
#include "SetTesterExceptions.h"
#include "Set.h"

class SetTester {
private:
    MainMemoryManager* _mem_manager;
    Set* _set;

    void _create_set();
    void _fill_set(size_t elem_count);
    void _destroy_set();

public:
    SetTester(size_t mem_bytes_size);

    void test_insert(size_t elem_count);
    void test_insert_duplicates(size_t elem_count);
    void test_find(size_t elem_count);
    void test_remove(size_t elem_count);
    void test_remove_even(size_t elem_count);
    void test_duplicated_iterator();

    void run_all_tests(size_t elem_count);
    
    ~SetTester();

    friend class SetTesterException;
};
