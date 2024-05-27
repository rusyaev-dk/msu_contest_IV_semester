#pragma once

#include "MainMemoryManager.h"
#include "Set.h"
#include "SetTesterExceptions.h"

class SetTester {
private:
    MainMemoryManager* _mem_manager;
    Set* _set;

    void _create_set();
    void _fill_set(size_t elem_count);
    void _validate_insertion_code(int err_code);
    void _destroy_set();

public:
    SetTester(size_t mem_bytes_size);

    void test_insert(size_t elem_count);
    void test_insert_duplicates(size_t elem_count);
    void test_find(size_t elem_count);
    void test_remove(size_t elem_count);
    void test_remove_even(size_t elem_count);
    void test_clear(size_t elem_count);
    void test_iterator(size_t elem_count);
    void test_duplicated_iterator();
    void test_user_data_type();

    void check_perfomance(size_t elem_count);
    void run_all_tests(size_t elem_count);
    
    ~SetTester();
};
