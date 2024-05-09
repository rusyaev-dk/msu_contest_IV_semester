#include "SetTester.h"
#include "SetTesterExceptions.h"
#include <chrono>

SetTester::SetTester(size_t mem_bytes_size) {
    this->_mem_manager = new MainMemoryManager(mem_bytes_size);
    this->_set = new Set(*this->_mem_manager); 
}

void SetTester::_fill_set(size_t elem_count) {
    int err_code;
    for (size_t i = 0; i < elem_count; i++) {
        err_code = this->_set->insert(&i, sizeof(i));
        if (err_code != 0) {
            this->_set->clear();
            throw SetTesterInsertException(&i, err_code);
        }
    }
}

void SetTester::insert_test(size_t elem_count) {
    auto start = std::chrono::high_resolution_clock::now();
    
    this->_fill_set(elem_count);

    auto finish = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed = finish - start;
    std::cout << "Inserted " << elem_count << " elements in " << elapsed.count() << " ms.\n";
    
    this->_set->clear();
}

void SetTester::find_test(size_t elem_count) {
    this->_fill_set(elem_count);

    auto start = std::chrono::high_resolution_clock::now();
    Set::Iterator* iter;
    for (size_t i = 0; i < elem_count; i++) {
        iter = this->_set->find(&i, sizeof(i));
        if (!iter) {
            this->_set->clear();
            throw SetTesterFindException(&i);
        }
        free(iter);
    }

    auto finish = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed = finish - start;
    std::cout << "Found " << elem_count << " elements in " << elapsed.count() << " ms.\n";
    
    this->_set->clear();
}

void SetTester::remove_even_test(size_t elem_count) {
    this->_fill_set(elem_count);

    auto start = std::chrono::high_resolution_clock::now();   

    size_t* elem;
    size_t elem_size, counter = 0;
    Set::Iterator* set_iter = this->_set->newIterator();
    Set::Iterator* checking_iter;
    bool isNotEven;
    do {
        elem = (size_t*)set_iter->getElement(elem_size);
        isNotEven = !elem || *elem % 2 != 0;
        if (isNotEven) {
            set_iter->goToNext();
            continue;
        }

        this->_set->remove(set_iter);
        checking_iter = this->_set->find(&elem, elem_size);
        if (checking_iter) {
            throw SetTesterRemoveException(elem);
        }
        counter++;
    } while (set_iter->hasNext());
    
    auto finish = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed = finish - start;
    std::cout << "Removed " << counter << " elements in " << elapsed.count() << " ms.\n";

    free(set_iter);
    this->_set->clear();
}

void SetTester::duplicated_iterator_test() {
    size_t elem = 123;
    int err_code = this->_set->insert(&elem, sizeof(elem));
    if (err_code != 0) {
        this->_set->clear();
        throw SetTesterInsertException(&elem, err_code);
    }

    Set::Iterator* iter_1 = this->_set->newIterator();
    Set::Iterator* iter_2 = this->_set->find(&elem, sizeof(elem));

    this->_set->remove(iter_1);
    this->_set->remove(iter_2);
 
    free(iter_1);
    free(iter_2);
    this->_set->clear();
}

void SetTester::iterator_test(size_t elem_count) {
    this->_fill_set(elem_count);
    size_t elem_to_remove = 5;

    Set::SetIterator* set_iter = dynamic_cast<Set::SetIterator*>(this->_set->find(&elem_to_remove, sizeof(elem_to_remove)));

    this->_set->remove(set_iter);
    this->_set->remove(set_iter);

    size_t set_size = this->_set->size();
    this->_set->clear();
}

void SetTester::run_all_tests(size_t elem_count) {
    try{
        this->insert_test(elem_count);
        this->find_test(elem_count);
        this->remove_even_test(elem_count);
        this->duplicated_iterator_test();
    } catch (SetTesterException exception) {

    }
    
}

SetTester::~SetTester() {
    delete this->_set;
    delete this->_mem_manager;
    // вызывать деструктор менеджера памяти
    // this->mem_manager->
}
