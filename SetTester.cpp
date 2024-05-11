#include "SetTester.h"

#include <chrono>
#include <functional>
#include <vector>

static const int _TOTAL_TESTS_COUNT = 5;

SetTester::SetTester(size_t mem_bytes_size) {
    this->_mem_manager = new MainMemoryManager(mem_bytes_size);
    this->_set = nullptr;
}

void SetTester::_create_set() {
    if (this->_set) {
        this->_destroy_set();
    }
    this->_set = new Set(*this->_mem_manager);
}

void SetTester::_destroy_set() {
    delete this->_set;
    this->_set = nullptr;
}

void SetTester::_fill_set(size_t elem_count) {
    int err_code;
    for (size_t i = 0; i < elem_count; i++) {
        err_code = this->_set->insert(&i, sizeof(i));
        switch (err_code) {
            case 1:
            this->_destroy_set();
            throw SetTesterException(DUPLICATE_INSERT_ERROR);

            case 2:
            this->_destroy_set();
            throw SetTesterException(INSERT_ERROR, "Couldn't insert elem");

            default:
            break;
        }
    }
}

void SetTester::insert_test(size_t elem_count) {
    this->_create_set();

    auto start = std::chrono::high_resolution_clock::now();
    this->_fill_set(elem_count);
    auto finish = std::chrono::high_resolution_clock::now();

    if (this->_set->size() != elem_count) {
        this->_destroy_set();
        throw SetTesterException(INSERT_ERROR, "Not all elems have been inserted!");
    }

    std::chrono::duration<double, std::milli> elapsed = finish - start;
    std::cout << "Inserted " << elem_count << " elements in " << elapsed.count() << " ms.\n";
    
    this->_destroy_set();
}

void SetTester::insert_duplicates_test(size_t elem_count) {
    this->_create_set();
    this->_fill_set(elem_count);
    
    try {
        this->_fill_set(elem_count);
    } catch (SetTesterException& e) {
        this->_destroy_set();
        return;
    }

    this->_destroy_set();
    throw SetTesterException(DUPLICATE_INSERT_ERROR);
}

void SetTester::find_test(size_t elem_count) {
    this->_create_set();
    this->_fill_set(elem_count);

    auto start = std::chrono::high_resolution_clock::now();
    Set::Iterator* iter;
    for (size_t i = 0; i < elem_count; i++) {
        iter = this->_set->find(&i, sizeof(i));
        if (!iter) {
            this->_destroy_set();
            throw SetTesterException(ELEM_NOT_FOUND_ERROR);
        }
        free(iter);
    }

    auto finish = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed = finish - start;
    std::cout << "Found " << elem_count << " elements in " << elapsed.count() << " ms.\n";
    
    this->_destroy_set();
}

void SetTester::remove_even_test(size_t elem_count) {
    this->_create_set();
    this->_fill_set(elem_count);

    auto start = std::chrono::high_resolution_clock::now();   

    size_t* elem;
    size_t elem_size, counter = 0;
    Set::Iterator* set_iter = this->_set->newIterator();
    Set::Iterator* checking_iter;
    bool elemIsNotEven;
    do {
        elem = (size_t*)set_iter->getElement(elem_size);
        elemIsNotEven = !elem || *elem % 2 != 0;
        if (elemIsNotEven) {
            set_iter->goToNext();
            continue;
        }

        this->_set->remove(set_iter);
        checking_iter = this->_set->find(&elem, elem_size);
        if (checking_iter) {
            free(set_iter);
            free(checking_iter);
            this->_destroy_set();
            throw SetTesterException(REMOVE_ERROR);
        }
        counter++;
    } while (set_iter->hasNext());
    
    auto finish = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed = finish - start;
    std::cout << "Removed " << counter << " elements in " << elapsed.count() << " ms.\n";

    free(set_iter);
    this->_destroy_set();
}

void SetTester::duplicated_iterator_test() {
    this->_create_set();
    
    size_t elem = 123;
    int err_code = this->_set->insert(&elem, sizeof(elem));
    switch (err_code) {
        case 1:
        this->_destroy_set();
        throw SetTesterException(DUPLICATE_INSERT_ERROR);

        case 2:
        this->_destroy_set();
        throw SetTesterException(UNKNOWN_ERROR, "Couldn't insert elem");
    
        default:
        break;
    }

    Set::Iterator* iter_1 = this->_set->newIterator();
    Set::Iterator* iter_2 = this->_set->newIterator();

    this->_set->remove(iter_1);
    this->_set->remove(iter_2);
 
    free(iter_1);
    free(iter_2);
    this->_destroy_set();
}

void SetTester::run_all_tests(size_t elem_count) {
    int passed_count = 0;
    try {
        this->insert_test(elem_count);
        std::cout << "✓ Insert test passed" << std::endl;
        passed_count++;
    } catch (const SetTesterException& e) {
        std::cerr << "Insert test failed: " << e.what() << std::endl;
    }

    try {
        this->insert_duplicates_test(elem_count);
        std::cout << "✓ Insert duplicates test passed" << std::endl;
        passed_count++;
    } catch (const SetTesterException& e) {
        std::cerr << "Insert duplicates test failed: " << e.what() << std::endl;
    }

    try {
        this->find_test(elem_count);
        std::cout << "✓ Find test passed" << std::endl;
        passed_count++;
    } catch (const SetTesterException& e) {
        std::cerr << "Find test failed: " << e.what() << std::endl;
    }

    try {
        this->remove_even_test(elem_count);
        std::cout << "✓ Remove even test passed" << std::endl;
        passed_count++;
    } catch (const SetTesterException& e) {
        std::cerr << "Remove even test failed: " << e.what() << std::endl;
    }

    try {
        this->duplicated_iterator_test();
        std::cout << "✓ Duplicated iterator test passed" << std::endl;
        passed_count++;
    } catch (const SetTesterException& e) {
        std::cerr << "Duplicated iterator test failed: " << e.what() << std::endl;
    }
    
    std::cout << "Passed " << passed_count << " of " << _TOTAL_TESTS_COUNT << " tests\n";
}

SetTester::~SetTester() {
    delete this->_set;
    delete this->_mem_manager;
    // вызывать деструктор менеджера памяти
    // this->mem_manager->
}
