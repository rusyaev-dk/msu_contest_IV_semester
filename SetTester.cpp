#include "SetTester.h"

#include <chrono>
#include <functional>
#include <vector>

using ErrorCode = ContainerTesterException::ErrorCode; 

SetTester::SetTester(size_t bytes_size) : BaseContainerTester(bytes_size) {
    this->_set = nullptr;
}

void SetTester::test_insert(size_t elem_count) {
    this->_create_container();
    this->_fill_container_with_size_t(elem_count);
    
    if (this->_set->size() != elem_count) {
        this->_destroy_container();
        throw ContainerTesterException(ErrorCode::INSERT_ERROR, "Not all elems have been inserted.");
    }

    this->_destroy_container();
}

void SetTester::test_insert_duplicates(size_t elem_count) {
    this->_create_container();
    this->_fill_container_with_size_t(elem_count);
    
    try {
        this->_fill_container_with_size_t(elem_count);
    } catch (ContainerTesterException& e) {
        this->_destroy_container();
        return;
    }

    this->_destroy_container();
    throw ContainerTesterException(ErrorCode::DUPLICATE_INSERT_ERROR);
}

void SetTester::test_find(size_t elem_count) {
    this->_create_container();
    this->_fill_container_with_size_t(elem_count);

    Set::SetIterator* iter;
    for (size_t i = 0; i < elem_count; i++) {
        iter = dynamic_cast<Set::SetIterator*>(this->_set->find(&i, sizeof(i)));
        if (!iter) {
            this->_destroy_container();
            throw ContainerTesterException(ErrorCode::ELEM_NOT_FOUND_ERROR);
        }        
        delete iter;
    }

    this->_destroy_container();
}

void SetTester::test_remove(size_t elem_count) {
    this->_create_container();
    this->_fill_container_with_size_t(elem_count);

    Set::SetIterator* iter = dynamic_cast<Set::SetIterator*>(this->_set->newIterator());
    Set::SetIterator* checking_iter = nullptr;
    size_t set_size = this->_set->size();
    size_t size_before_remove = set_size, size_after_remove;
    
    for (size_t i = 0; i < set_size; i++) {
        size_t elem_size;
        void* elem = iter->getElement(elem_size);
        size_t elem_copy = *static_cast<size_t*>(elem);

        this->_set->remove(iter);

        size_after_remove = this->_set->size();
        if (size_after_remove >= size_before_remove) {
            delete iter;
            this->_destroy_container();
            throw ContainerTesterException(ErrorCode::REMOVE_ERROR, "The size of the set was not decreased after remove.");
        }
        size_before_remove = size_after_remove;
        
        checking_iter = dynamic_cast<Set::SetIterator*>(this->_set->find(&elem_copy, elem_size));
        if (checking_iter) {
            delete iter;
            delete checking_iter;
            this->_destroy_container();
            throw ContainerTesterException(ErrorCode::REMOVE_ERROR, "The element was not deleted.");
        }
    }
    delete iter;
    
    if (!this->_set->empty()) {
        this->_destroy_container();
        throw ContainerTesterException(ErrorCode::REMOVE_ERROR, "Set is not empty but it has to.");
    }

    this->_destroy_container();
}

void SetTester::test_remove_even(size_t elem_count) {
    this->_create_container();
    this->_fill_container_with_size_t(elem_count);

    Set::SetIterator* iter = dynamic_cast<Set::SetIterator*>(this->_set->newIterator());
    Set::SetIterator* checking_iter = nullptr;
    size_t set_size = this->_set->size();
    size_t size_before_remove = set_size, size_after_remove;

    for (size_t i = 0; i < set_size; i++) {
        size_t elem_size;
        size_t* elem = (size_t*)iter->getElement(elem_size);
        size_t elem_copy = *static_cast<size_t*>(elem);

        bool elem_is_odd = !elem || *elem % 2 != 0;
        if (elem_is_odd) {
            iter->goToNext();
            continue;
        }

        this->_set->remove(iter);

        size_after_remove = this->_set->size();
        if (size_after_remove >= size_before_remove) {
            delete iter;
            this->_destroy_container();
            throw ContainerTesterException(ErrorCode::REMOVE_ERROR, "The size of the set was not decreased after remove.");
        }
        size_before_remove = size_after_remove;
        
        checking_iter = dynamic_cast<Set::SetIterator*>(this->_set->find(&elem_copy, elem_size));
        if (checking_iter) {
            delete iter;
            delete checking_iter;
            this->_destroy_container();
            throw ContainerTesterException(ErrorCode::REMOVE_ERROR, "The element was not deleted.");
        }
    }
    delete iter;

    if (this->_set->size() != elem_count / 2) {        
        this->_destroy_container();
        throw ContainerTesterException(ErrorCode::REMOVE_ERROR, "Not all even elements have been removed.");
    }

    this->_destroy_container();
}

void SetTester::test_clear(size_t elem_count) {
    this->_create_container();
    this->_fill_container_with_size_t(elem_count);
    
    this->_set->clear();
    try {
        this->_fill_container_with_size_t(elem_count);
    } catch (ContainerTesterException& e) {
        this->_destroy_container();
        throw ContainerTesterException(ErrorCode::CLEAR_ERROR, "There are some found elems in set after it's cleaned.");
    }

    this->_set->clear();
    Set::SetIterator* iter;
    for (size_t i = 0; i < elem_count; i++) {
        iter = dynamic_cast<Set::SetIterator*>(this->_set->find(&i, sizeof(i)));
        if (iter) {
            delete iter;
            this->_destroy_container();
            throw ContainerTesterException(ErrorCode::CLEAR_ERROR, "There are some found elems in set after it's cleaned.");
        }
    }

    this->_destroy_container();
}

void SetTester::test_iterator_traversal(size_t elem_count) {
    this->_create_container();
    this->_fill_container_with_size_t(elem_count);

    Set::SetIterator* iter = dynamic_cast<Set::SetIterator*>(this->_set->newIterator());
    size_t set_size = this->_set->size();
    for (size_t i = 0; i < set_size; i++) {
        iter->goToNext();
        size_t elem_size;
        void* elem = iter->getElement(elem_size);
        if (!elem) {
            delete iter;
            this->_destroy_container();
            throw ContainerTesterException(ErrorCode::ITERATOR_ERROR, "Address of elem is null.");
        }
    }

    delete iter;
    this->_destroy_container();
}

void SetTester::test_iterator_after_last_elem_removal() {
    this->_create_container();
    this->_fill_container_with_size_t(1);
    
    Set::SetIterator* iter = dynamic_cast<Set::SetIterator*>(this->_set->newIterator());
    if (!iter) {
        this->_destroy_container();
        throw ContainerTesterException(ErrorCode::ITERATOR_ERROR, "No iterator created for non-empty set.");
    }
    this->_set->remove(iter);
    size_t elem_size;
    void* elem = iter->getElement(elem_size);
    if (elem) {
        delete iter;
        this->_destroy_container();
        throw ContainerTesterException(ErrorCode::ITERATOR_ERROR, "Iterator has elem after removing last elem in set.");
    }

    delete iter;    
    this->_destroy_container();
}

void SetTester::test_iterator_empty_set() {
    this->_create_container();

    try {
        Set::SetIterator* iter = dynamic_cast<Set::SetIterator*>(this->_set->newIterator());
        if (iter) {
            this->_destroy_container();
            throw ContainerTesterException(ErrorCode::ITERATOR_ERROR, "Created iter for empty set.");
        }
    } catch (Container::Error& e) {
        this->_destroy_container();
        throw ContainerTesterException(ErrorCode::ITERATOR_ERROR, e.msg);
    }
    
    this->_destroy_container();
}

void SetTester::test_iterator_cleared_set(size_t elem_count) {
    this->_create_container();
    this->_fill_container_with_size_t(elem_count);

    Set::SetIterator* iter = dynamic_cast<Set::SetIterator*>(this->_set->newIterator());
    this->_set->clear();

    size_t elem_size;
    void* elem = iter->getElement(elem_size);
    if (elem) {
        delete iter;
        this->_destroy_container();
        throw ContainerTesterException(ErrorCode::ITERATOR_ERROR, "There is an elem in iterator of cleared set.");
    }

    delete iter;
    this->_destroy_container();
}

void SetTester::test_duplicate_iterators_removal() {
    this->_create_container();
    
    size_t elem = 123;
    int err_code = this->_set->insert(&elem, sizeof(elem));
    this->_validate_insertion_code(err_code);

    Set::SetIterator* iter_1 = dynamic_cast<Set::SetIterator*>(this->_set->newIterator());
    Set::SetIterator* iter_2 = dynamic_cast<Set::SetIterator*>(this->_set->newIterator());
   
    this->_set->remove(iter_1);
    this->_set->remove(iter_2);
 
    delete iter_1;
    delete iter_2;
    this->_destroy_container();
}

void SetTester::test_user_data_type() {
    this->_create_container();
    typedef struct {
        double x, y;
    } Point;
    Point p1 = { 123, -0.3423 };

    int err_code = this->_set->insert(&p1, sizeof(p1));
    this->_validate_insertion_code(err_code);

    Set::SetIterator* iter = dynamic_cast<Set::SetIterator*>(this->_set->find(&p1, sizeof(p1)));
    if (!iter) {
        this->_destroy_container();
        throw ContainerTesterException(ErrorCode::ELEM_NOT_FOUND_ERROR);
    }
    size_t p_size;
    Point* p = (Point*)iter->getElement(p_size);

    if (p->x != p1.x || p->y != p1.y || p_size != sizeof(Point)) {
        delete iter;
        this->_destroy_container();
        throw ContainerTesterException(ErrorCode::DATA_ERROR, "Incorrect saving user data type.");
    }

    this->_set->remove(iter);

    delete iter;
    this->_destroy_container();
}

void SetTester::check_perfomance(size_t elem_count) {
    using namespace std;
    this->_create_container();
    
    auto start = chrono::high_resolution_clock::now();
    for (size_t i = 0; i < elem_count; i++) {
        this->_set->insert(&i, sizeof(i));
    }
    
    auto finish = chrono::high_resolution_clock::now(); 
    chrono::duration<double, milli> elapsed = finish - start;
    cout << "Inserted " << elem_count << " elements in " << elapsed.count() << " ms.\n";

    Set::SetIterator* iter;
    start = chrono::high_resolution_clock::now();
    for (size_t i = 0; i < elem_count; i++) {
        iter = dynamic_cast<Set::SetIterator*>(this->_set->find(&i, sizeof(i)));
        if (iter) delete iter;
    }
    
    finish = chrono::high_resolution_clock::now();
    elapsed = finish - start;
    cout << "Found " << elem_count << " elements in " << elapsed.count() << " ms.\n";

    iter = dynamic_cast<Set::SetIterator*>(this->_set->newIterator());
    size_t set_size = this->_set->size();
    start = chrono::high_resolution_clock::now();
    for (size_t i = 0; i < set_size; i++) {
        size_t elem_size;
        void* elem = iter->getElement(elem_size);

        this->_set->remove(iter);
    }

    finish = chrono::high_resolution_clock::now();
    elapsed = finish - start;
    cout << "Removed " << elem_count << " elements in " << elapsed.count() << " ms.\n";

    this->_destroy_container(); 
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
        } catch (const ContainerTesterException& e) {
            std::cout << "⚠️ " << test_name << " failed: " << e.what() << std::endl;
        } catch (const Container::Error& e) {
            std::cout << "⚠️ " << test_name << " failed: " << e.msg << std::endl;
        }
    };
    
    for (const auto& test : tests) {
        run_test(test.first, test.second);
    }

    std::cout << "Passed " << passed_count << " of " << tests.size() << " tests." << std::endl;
}

void SetTester::_create_container() {
    if (this->_set) this->_destroy_container();
    this->_set = new Set(*this->_mem_manager);
}

void SetTester::_fill_container_with_size_t(size_t elem_count) {
    size_t size_before_insert = 0, size_after_insert;
    
    for (size_t i = 0; i < elem_count; i++) {
        int err_code = this->_set->insert(&i, sizeof(i));
        this->_validate_insertion_code(err_code);

        size_after_insert = this->_set->size();
        if (size_before_insert >= size_after_insert) {
            this->_destroy_container();
            throw ContainerTesterException(ErrorCode::INSERT_ERROR, "The size of the set was not increased after insert.");
        }
    }
}

void SetTester::_destroy_container() {
    if (!this->_set) return;
    delete this->_set;
    this->_set = nullptr;
}

SetTester::~SetTester() {
    delete this->_set;
    // ~BaseContainerTester();
}
