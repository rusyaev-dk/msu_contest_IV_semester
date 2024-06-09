#pragma once

#include "Set.h"
#include "BaseContainerTester.h"

#include <functional>

class SetTester : BaseContainerTester {
private:
    Set* _set;
    
    void _create_container() override;
    void _fill_container_with_size_t(size_t elem_count) override;
    void _destroy_container() override;
    void _container_traversal_with_check(size_t elem_count, function<bool(size_t)> rule);

public:
    SetTester(size_t bytes_size);

    void test_insert(size_t elem_count) override;
    void test_insert_duplicates(size_t elem_count);
    
    void test_find(size_t elem_count) override;
    
    void test_remove(size_t elem_count) override;
    void test_remove_even(size_t elem_count);

    void test_clear(size_t elem_count) override;
    
    void test_iterator_after_last_elem_removal();
    void test_iterator_after_set_cleared(size_t elem_count);
    void test_iterator_empty_set();
    void test_duplicate_iterators_removal();
    
    void test_user_data_type();

    void check_perfomance(size_t elem_count);
    void run_all_tests(size_t elem_count);
    
    ~SetTester();
};
