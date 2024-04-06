#include "SetTester.h"

SetTester::SetTester(size_t mem_size) {
    MainMemoryManager* mem_manager = new MainMemoryManager(mem_size);
    this->_mem_manager = mem_manager;
    Set* new_set = new Set(*this->_mem_manager);
    this->_set = new_set; 
}

int SetTester::insert_test(int test_elem_count) {
    
}

SetTester::~SetTester() {
    this->_set->~Set();
    free(this->_set);
    free(this->_mem_manager);
    // вызывать деструктор менеджера памяти
    // this->mem_manager->
}
