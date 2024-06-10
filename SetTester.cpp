#include "SetTester.h"

#include <chrono>
#include <vector>

using ErrorCode = ContainerTesterException::ErrorCode;
using namespace std;

SetTester::SetTester(size_t bytes_size) : BaseContainerTester(bytes_size) {
    this->_set = nullptr;
}

void SetTester::test_insert(size_t elem_count) {
    this->_create_container();
    this->_fill_container_with_size_t(elem_count);
    
    if (this->_set->size() != elem_count) {
        this->_destroy_container();
        throw ContainerTesterException(ErrorCode::INSERT_ERROR, "Set size was not properly increased after insetring.");
    }
    
    auto rule = [elem_count](size_t elem) { return (elem >= 0 && elem <= elem_count); };
    this->_container_traversal_with_check(elem_count, rule);

    this->_destroy_container();
}

void SetTester::test_insert_duplicates(size_t elem_count) {
    this->_create_container();
    this->_fill_container_with_size_t(elem_count);
    
    try {
        this->_fill_container_with_size_t(elem_count);
    } catch (ContainerTesterException& e) {
        return;
    }

    this->_destroy_container();
    throw ContainerTesterException(ErrorCode::DUPLICATE_INSERT_ERROR);
}

void SetTester::test_find(size_t elem_count) {
    this->_create_container();
    this->_fill_container_with_size_t(elem_count);

    Set::SetIterator* iter;
    size_t uninserted_elem = elem_count + 1;
    iter = dynamic_cast<Set::SetIterator*>(this->_set->find(&uninserted_elem, sizeof(uninserted_elem)));
    if (iter) {
        delete iter;
        this->_destroy_container();
        throw ContainerTesterException(ErrorCode::DATA_ERROR, "Found uninserted elem in set.");
    }

    for (size_t i = 0; i < elem_count; i++) {
        iter = dynamic_cast<Set::SetIterator*>(this->_set->find(&i, sizeof(i)));
        if (!iter) {
            this->_destroy_container();
            throw ContainerTesterException(ErrorCode::ELEM_NOT_FOUND_ERROR, "Iterator for exact elem was not found.");
        }

        size_t elem_size;
        size_t* elem = (size_t*)iter->getElement(elem_size);
        delete iter;
        if (*elem != i) {
            this->_destroy_container();
            throw ContainerTesterException(ErrorCode::INSERT_ERROR, "Elem was incorrectly inserted.");
        }
    }

    this->_destroy_container();
}

void SetTester::test_remove(size_t elem_count) {
    this->_create_container();
    this->_fill_container_with_size_t(elem_count);

    Set::SetIterator* iter = dynamic_cast<Set::SetIterator*>(this->_set->newIterator());
    Set::SetIterator* checking_iter = nullptr;
    size_t set_size = this->_set->size();
    
    for (size_t i = 0; i < set_size; i++) {
        size_t elem_size;
        void* elem = iter->getElement(elem_size);
        size_t* elem_copy = (size_t*)(elem);

        this->_set->remove(iter);
        
        checking_iter = dynamic_cast<Set::SetIterator*>(this->_set->find(elem_copy, elem_size));
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
    size_t initial_set_size = this->_set->size();

    for (size_t i = 0; i < initial_set_size; i++) {
        size_t elem_size;
        size_t* elem = (size_t*)iter->getElement(elem_size);

        bool elem_is_uneven = (*elem % 2 != 0);
        if (elem_is_uneven) {
            iter->goToNext();
            continue;
        }
        this->_set->remove(iter);
    }
    delete iter;

    if (this->_set->size() != elem_count / 2) {        
        this->_destroy_container();
        throw ContainerTesterException(ErrorCode::REMOVE_ERROR, "Set size was not correctly decreased after removal.");
    }

    auto rule = [elem_count](size_t elem) { return ((elem > 0 && elem <= elem_count) && (elem % 2 != 0)); };
    this->_container_traversal_with_check(elem_count, rule);
    
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
    delete iter;
    if (elem) {
        this->_destroy_container();
        throw ContainerTesterException(ErrorCode::ITERATOR_ERROR, "Iterator has elem after removing last elem in set.");
    }
    
    this->_destroy_container();
}

void SetTester::test_iterator_empty_set() {
    this->_create_container();

    try {
        Set::SetIterator* iter = dynamic_cast<Set::SetIterator*>(this->_set->newIterator());
        if (iter) {
            delete iter;
            this->_destroy_container();
            throw ContainerTesterException(ErrorCode::ITERATOR_ERROR, "Created iter for empty set.");
        }
    } catch (Container::Error& e) {
        this->_destroy_container();
        throw ContainerTesterException(ErrorCode::ITERATOR_ERROR, e.msg);
    }
    
    this->_destroy_container();
}

void SetTester::test_iterator_after_set_cleared(size_t elem_count) {
    this->_create_container();
    this->_fill_container_with_size_t(elem_count);

    Set::SetIterator* iter = dynamic_cast<Set::SetIterator*>(this->_set->newIterator());
    this->_set->clear();

    size_t elem_size;
    void* elem = iter->getElement(elem_size);
    if (elem) {
        delete iter;
        this->_destroy_container();
        throw ContainerTesterException(ErrorCode::ITERATOR_ERROR, "There is active elem in iterator after set cleared.");
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
    size_t elem_size;
    size_t* removed_elem = (size_t*)iter_2->getElement(elem_size);
    if (removed_elem) {
        delete iter_1;
        delete iter_2;
        this->_destroy_container();
        throw ContainerTesterException(ErrorCode::ITERATOR_ERROR, "Removed elem is active in another iterator.");
    }

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
    const initializer_list<pair<function<void()>, string>> tests = {
        {[&] { this->test_insert(elem_count); }, "Insert test"},
        {[&] { this->test_insert_duplicates(elem_count); }, "Insert duplicates test"},
        {[&] { this->test_find(elem_count); }, "Find test"},
        {[&] { this->test_remove(elem_count); }, "Remove test"},
        {[&] { this->test_remove_even(elem_count); }, "Remove even test"},
        {[&] { this->test_clear(elem_count); }, "Clear test"},
        {[&] { this->test_iterator_after_last_elem_removal(); }, "Iterator last elem removal test"},
        {[&] { this->test_iterator_after_set_cleared(elem_count); }, "Iterator cleared set test"},
        {[&] { this->test_iterator_empty_set(); }, "Iterator empty set test"},
        {[&] { this->test_duplicate_iterators_removal(); }, "Duplicate iterators removal test"},
        {[&] { this->test_user_data_type(); }, "User data type test"},
    };

    int passed_count = 0;
    auto run_test = [&](auto test_func, const string& test_name) {
        try {
            test_func();
            cout << "✓ " << test_name << " passed\n\n";
            passed_count++;
        } catch (const ContainerTesterException& e) {
            cout << "⚠️ " << test_name << " failed: " << e.what() << "\n\n";
        } catch (const Container::Error& e) {
            cout << "⚠️ " << test_name << " failed: " << e.msg << "\n\n";
        }
    };
    
    for (const auto& test : tests) {
        run_test(test.first, test.second);
    }

    cout << "Passed " << passed_count << " of " << tests.size() << " tests." << "\n";
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

void SetTester::_container_traversal_with_check(size_t elem_count, function<bool(size_t)> rule) {
    vector<pair<size_t, bool>> elements_to_view(elem_count);
    for (size_t i = 0; i < elem_count; i++) {
        elements_to_view[i] = make_pair(i, false);
    }

    Set::SetIterator* iter = dynamic_cast<Set::SetIterator*>(this->_set->newIterator());
    size_t set_size = this->_set->size();
    try {
        for (size_t i = 0; i < set_size; i++) {
            size_t elem_size, index;
            size_t* elem = (size_t*)iter->getElement(elem_size);
            index = *elem;

            if (!elem) {
                throw ContainerTesterException(ErrorCode::ITERATOR_ERROR, "Address of elem is null.");
            }

            if (!rule(*elem)) {
                throw ContainerTesterException(ErrorCode::DATA_ERROR, "The element that does not match the rule remains.");
            }

            if (elements_to_view[index].second) {        
                throw ContainerTesterException(ErrorCode::DUPLICATE_INSERT_ERROR, "Duplicate elements in set.");
            }

            elements_to_view[index].second = true;
            iter->goToNext();
            elem = nullptr;
        }
        delete iter;
    } catch (const ContainerTesterException& e) {
        delete iter;
        this->_destroy_container();
        throw;
    }
    
    for (size_t i = 0; i < elem_count; i++) {
        if (!elements_to_view[i].second && rule(i)) {
            throw ContainerTesterException(ErrorCode::DATA_ERROR, "Not all elements were found.");
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
}
