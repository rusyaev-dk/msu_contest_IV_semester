#pragma once
//#include "MainMemoryManager.h"
#include "Table.h"
#include "BaseContainerTester.h"
#include "ContainerTesterException.h"

class TableTest : BaseContainerTester{
private:
	Table* table;
    int numElements;

    void _create_container() override;
    void _fill_container_with_size_t(size_t elem_count) override;
    void _destroy_container() override;

public:

	TableTest(size_t bytesSize, int numElements);
    //заключить все в проверку
    void test_insert(size_t elem_count) override;
    void testRemoveByKey();   
    void testFindByKey();
    void testAt();
    void testSize();
    void test_find(size_t elem_count) override;
    void test_remove(size_t elem_count) override;
    void test_clear(size_t elem_count) override;
    void testEmpty();

   

    ~TableTest();
};