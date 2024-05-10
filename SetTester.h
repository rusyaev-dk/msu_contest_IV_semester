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

    void insert_test(size_t elem_count);
    // void insert_duplicates_test(size_t elem_count);
    void find_test(size_t elem_count);
    void remove_even_test(size_t elem_count);
    void duplicated_iterator_test();
    void iterator_test(size_t elem_count);

    void run_all_tests(size_t elem_count);
    
    ~SetTester();

    friend class SetTesterException;
};
