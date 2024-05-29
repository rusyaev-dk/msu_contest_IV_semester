#include "SetTester.h"

#include <chrono>
#include <functional>
#include <vector>

using ErrorCode = SetTesterException::ErrorCode; 

SetTester::SetTester(size_t mem_bytes_size) {
    this->_mem_manager = new MainMemoryManager(mem_bytes_size);
    this->_set = nullptr;
}

void SetTester::test_insert(size_t elem_count) {
    this->_create_set();
    this->_fill_set_with_int(elem_count);
    
    if (this->_set->size() != elem_count) {
        this->_destroy_set();
        throw SetTesterException(ErrorCode::INSERT_ERROR, "Not all elems have been inserted!");
    }

    this->_destroy_set();
}

void SetTester::test_insert_duplicates(size_t elem_count) {
    this->_create_set();
    this->_fill_set_with_int(elem_count);
    
    try {
        this->_fill_set_with_int(elem_count);
    } catch (SetTesterException& e) {
        this->_destroy_set();
        return;
    }

    this->_destroy_set();
    throw SetTesterException(ErrorCode::DUPLICATE_INSERT_ERROR);
}

void SetTester::test_find(size_t elem_count) {
    this->_create_set();
    this->_fill_set_with_int(elem_count);

    Set::Iterator* iter;
    for (size_t i = 0; i < elem_count; i++) {
        iter = this->_set->find(&i, sizeof(i));
        if (!iter) {
            this->_destroy_set();
            throw SetTesterException(ErrorCode::ELEM_NOT_FOUND_ERROR);
        }
        delete iter;
    }

    this->_destroy_set();
}

void SetTester::test_remove(size_t elem_count) {
    this->_create_set();
    this->_fill_set_with_int(elem_count);

    Set::Iterator* iter = this->_set->newIterator();
    Set::Iterator* checking_iter = nullptr;
    size_t set_size = this->_set->size();
    size_t size_before_remove = set_size, size_after_remove;
    
    for (size_t i = 0; i < set_size; i++) {
        size_t elem_size;
        void* elem = iter->getElement(elem_size);

        this->_set->remove(iter);

        size_after_remove = this->_set->size();
        if (size_after_remove >= size_before_remove) {
            delete iter;
            this->_destroy_set();
            throw SetTesterException(ErrorCode::REMOVE_ERROR, "The size of the set was not decreased after remove.");
        }
        size_before_remove = size_after_remove;
        
        checking_iter = this->_set->find(elem, elem_size);
        if (checking_iter) {
            delete iter;
            delete checking_iter;
            this->_destroy_set();
            throw SetTesterException(ErrorCode::REMOVE_ERROR, "The element was not deleted.");
        }
    }
    delete iter;
    
    if (!this->_set->empty()) {
        this->_destroy_set();
        throw SetTesterException(ErrorCode::REMOVE_ERROR, "Set is not empty but it has to...");
    }

    this->_destroy_set();
}

void SetTester::test_remove_even(size_t elem_count) {
    this->_create_set();
    this->_fill_set_with_int(elem_count);

    Set::Iterator* iter = this->_set->newIterator();
    Set::Iterator* checking_iter = nullptr;
    size_t set_size = this->_set->size();
    size_t size_before_remove = set_size, size_after_remove;

    for (size_t i = 0; i < set_size; i++) {
        size_t elem_size;
        size_t* elem = (size_t*)iter->getElement(elem_size);
        bool elem_is_odd = !elem || *elem % 2 != 0;
        if (elem_is_odd) {
            iter->goToNext();
            continue;
        }

        this->_set->remove(iter);

        size_after_remove = this->_set->size();
        if (size_after_remove >= size_before_remove) {
            delete iter;
            this->_destroy_set();
            throw SetTesterException(ErrorCode::REMOVE_ERROR, "The size of the set was not decreased after remove.");
        }
        size_before_remove = size_after_remove;
        
        checking_iter = this->_set->find(elem, elem_size);
        if (checking_iter) {
            delete iter;
            delete checking_iter;
            this->_destroy_set();
            throw SetTesterException(ErrorCode::REMOVE_ERROR, "The element was not deleted.");
        }
    }
    delete iter;

    if (this->_set->size() != elem_count / 2) {        
        this->_destroy_set();
        throw SetTesterException(ErrorCode::REMOVE_ERROR, "Not all even elements have been removed.");
    }

    this->_destroy_set();
}

void SetTester::test_clear(size_t elem_count) {
    this->_create_set();
    this->_fill_set_with_int(elem_count);
    
    this->_set->clear();
    try {
        this->_fill_set_with_int(elem_count);
    } catch (SetTesterException& e) {
        this->_destroy_set();
        throw SetTesterException(ErrorCode::CLEAR_ERROR, "There are some found elems in set after it's cleaned.");
    }

    this->_set->clear();
    Set::Iterator* iter;
    for (size_t i = 0; i < elem_count; i++) {
        iter = this->_set->find(&i, sizeof(i));
        if (iter) {
            delete iter;
            this->_destroy_set();
            throw SetTesterException(ErrorCode::CLEAR_ERROR, "There are some found elems in set after it's cleaned.");
        }
    }

    this->_destroy_set();
}

void SetTester::test_iterator_traversal(size_t elem_count) {
    this->_create_set();
    this->_fill_set_with_int(elem_count);

    Set::Iterator* iter = this->_set->newIterator();
    size_t set_size = this->_set->size();
    for (size_t i = 0; i < set_size; i++) {
        iter->goToNext();
        size_t elem_size;
        void* elem = iter->getElement(elem_size);
        if (!elem) {
            delete iter;
            this->_destroy_set();
            throw SetTesterException(ErrorCode::ITERATOR_ERROR, "Address of elem is null.");
        }
    }

    delete iter;
    this->_destroy_set();
}

void SetTester::test_iterator_after_last_elem_removal() {
    this->_create_set();
    this->_fill_set_with_int(1);
    
    Set::Iterator* iter = this->_set->newIterator();
    if (!iter) {
        this->_destroy_set();
        throw SetTesterException(ErrorCode::ITERATOR_ERROR, "No iterator created for non-empty set.");
    }
    this->_set->remove(iter);
    size_t elem_size;
    void* elem = iter->getElement(elem_size);
    if (elem) {
        delete iter;
        this->_destroy_set();
        throw SetTesterException(ErrorCode::ITERATOR_ERROR, "Iterator has elem after removing last elem in set.");
    }

    delete iter;    
    this->_destroy_set();
}

void SetTester::test_iterator_empty_set() {
    this->_create_set();

    try {
        Set::Iterator* iter = this->_set->newIterator();
        if (iter) {
            this->_destroy_set();
            throw SetTesterException(ErrorCode::ITERATOR_ERROR, "Created iter for empty set.");
        }
    } catch (Container::Error& e) {
        this->_destroy_set();
        throw SetTesterException(ErrorCode::ITERATOR_ERROR, e.msg);
    }
    
    this->_destroy_set();
}

void SetTester::test_iterator_cleared_set(size_t elem_count) {
    this->_create_set();
    this->_fill_set_with_int(elem_count);

    Set::Iterator* iter = this->_set->newIterator();
    this->_set->clear();

    size_t elem_size;
    void* elem = iter->getElement(elem_size);
    if (elem) {
        delete iter;
        this->_destroy_set();
        throw SetTesterException(ErrorCode::ITERATOR_ERROR, "There is an elem in iterator of cleared set.");
    }

    delete iter;
    this->_destroy_set();
}

void SetTester::test_duplicate_iterators_removal() {
    this->_create_set();
    
    size_t elem = 123;
    int err_code = this->_set->insert(&elem, sizeof(elem));
    this->_validate_insertion_code(err_code);

    Set::Iterator* iter_1 = this->_set->newIterator();
    Set::Iterator* iter_2 = this->_set->newIterator();

    this->_set->remove(iter_1);
    this->_set->remove(iter_2);
 
    delete iter_1;
    delete iter_2;
    this->_destroy_set();
}

void SetTester::test_user_data_type() {
    typedef struct {
        double x, y;
    } Point;

    this->_create_set();
    Point p1 = { 123, -0.3423 };

    int err_code = this->_set->insert(&p1, sizeof(p1));
    this->_validate_insertion_code(err_code);

    Set::Iterator* iter = this->_set->find(&p1, sizeof(p1));
    if (!iter) {
        this->_destroy_set();
        throw SetTesterException(ErrorCode::ELEM_NOT_FOUND_ERROR);
    }
    this->_set->remove(iter);
    
    delete iter;
    this->_destroy_set();
}

void SetTester::check_perfomance(size_t elem_count) {
    this->_create_set();
    
    auto start = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < elem_count; i++) {
        this->_set->insert(&i, sizeof(i));
    }
    
    auto finish = std::chrono::high_resolution_clock::now(); 
    std::chrono::duration<double, std::milli> elapsed = finish - start;
    std::cout << "Inserted " << elem_count << " elements in " << elapsed.count() << " ms.\n";

    Set::Iterator* iter;
    start = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < elem_count; i++) {
        iter = this->_set->find(&i, sizeof(i));
        if (iter) delete iter;
    }
    
    finish = std::chrono::high_resolution_clock::now();
    elapsed = finish - start;
    std::cout << "Found " << elem_count << " elements in " << elapsed.count() << " ms.\n";

    iter = this->_set->newIterator();
    size_t set_size = this->_set->size();
    start = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < set_size; i++) {
        size_t elem_size;
        void* elem = iter->getElement(elem_size);

        this->_set->remove(iter);
    }

    finish = std::chrono::high_resolution_clock::now();
    elapsed = finish - start;
    std::cout << "Removed " << elem_count << " elements in " << elapsed.count() << " ms.\n";

    this->_destroy_set(); 
}

void SetTester::run_all_tests(size_t elem_count) {
    // tests is vector containing pairs (&test_func, test_name)
    const std::initializer_list<std::pair<std::function<void()>, std::string>> tests = {
        {[&] { this->test_insert(elem_count); }, "Insert test"},
        {[&] { this->test_insert_duplicates(elem_count); }, "Insert duplicates test"},
        {[&] { this->test_find(elem_count); }, "Find test"},
        {[&] { this->test_remove(elem_count); }, "Remove test"},
        {[&] { this->test_remove_even(elem_count); }, "Remove even test"},
        {[&] { this->test_clear(elem_count); }, "Clear test"},
        {[&] { this->test_iterator_traversal(elem_count); }, "Iterator traversal test"},
        {[&] { this->test_iterator_after_last_elem_removal(); }, "Iterator last elem removal test"},
        {[&] { this->test_iterator_empty_set(); }, "Iterator empty set test"},
        {[&] { this->test_iterator_cleared_set(elem_count); }, "Iterator cleared set test"},
        {[&] { this->test_duplicate_iterators_removal(); }, "Duplicate iterators removal test"},
        {[&] { this->test_user_data_type(); }, "User data type test"},
    };

    int passed_count = 0;
    auto run_test = [&](auto test_func, const std::string& test_name) {
        try {
            test_func();
            std::cout << "✓ " << test_name << " passed" << std::endl;
            passed_count++;
        } catch (const SetTesterException& e) {
            std::cout << "⚠️ " << test_name << " failed: " << e.what() << std::endl;
        } catch (const Container::Error& e) {
            std::cout << "⚠️ " << test_name << " failed: " << e.msg << std::endl;
        }
    };
    
    for (const auto& test : tests) {
        run_test(test.first, test.second);
    }

    std::cout << "Passed " << passed_count << " of " << tests.size() << " tests\n";
}

void SetTester::_create_set() {
    if (this->_set) {
        this->_destroy_set();
    }
    this->_set = new Set(*this->_mem_manager);
}

void SetTester::_fill_set_with_int(size_t elem_count) {
    size_t size_before_insert = 0, size_after_insert;
    for (size_t i = 0; i < elem_count; i++) {
        int err_code = this->_set->insert(&i, sizeof(i));
        this->_validate_insertion_code(err_code);

        size_after_insert = this->_set->size();
        if (size_before_insert >= size_after_insert) {
            this->_destroy_set();
            throw SetTesterException(ErrorCode::INSERT_ERROR, "The size of the set was not increased after insert.");
        }
    }
}

void SetTester::_validate_insertion_code(int err_code) {
    switch (err_code) {
        case 1:
        this->_destroy_set();
        throw SetTesterException(ErrorCode::DUPLICATE_INSERT_ERROR);

        case 2:
        this->_destroy_set();
        throw SetTesterException(ErrorCode::INSERT_ERROR, "The element was not inserted.");

        default:
        break;
    }
}

void SetTester::_destroy_set() {
    delete this->_set;
    this->_set = nullptr;
}

SetTester::~SetTester() {
    delete this->_set;
    delete this->_mem_manager;
}
