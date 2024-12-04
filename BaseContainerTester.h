#pragma once

#include "Container.h"
#include "ContainerTesterException.h"
#include "Mem.h"

class BaseContainerTester {
protected:
    Mem* _mem_manager;

    virtual void _create_container() = 0;
    virtual void _fill_container_with_size_t(size_t elem_count) = 0;
    virtual void _destroy_container() = 0;

    void _validate_insertion_code(int err_code);

public:

    BaseContainerTester(size_t bytes_size);

    virtual void test_insert(size_t elem_count) = 0;
    virtual void test_remove(size_t elem_count) = 0;
    virtual void test_find(size_t elem_count) = 0;
    virtual void test_clear(size_t elem_count) = 0;

    ~BaseContainerTester();
};
