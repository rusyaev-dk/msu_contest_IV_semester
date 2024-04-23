#include "SetTester.h"
#include "SetTesterExceptions.h"
#include <chrono>

SetTester::SetTester(size_t mem_bytes_size) {
    MainMemoryManager* mem_manager = new MainMemoryManager(mem_bytes_size);
    this->_mem_manager = mem_manager;
    Set* new_set = new Set(*this->_mem_manager);
    this->_set = new_set; 
}

void SetTester::insert_test() {
    auto start = std::chrono::high_resolution_clock::now();
    
    for (size_t i = 0; i < this->_set->_set_data_len; i++) {
        size_t new_elem = i;
        size_t new_elem_size = sizeof(new_elem);
        
        int insert_err = this->_set->insert(&new_elem, new_elem_size);
        if (insert_err != 0) {
            this->_set->clear();
            throw SetTesterInsertException(i, &new_elem, insert_err);
        }
    }

    auto finish = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed = finish - start;
    std::cout << "Insertred " << this->_set->_set_data_len << " elements in " << elapsed.count() << " ms.\n";
    
    this->_set->clear();
}

void SetTester::find_test() {
    for (size_t i = 0; i < this->_set->_set_data_len; i++) {
        size_t new_elem = i;
        size_t new_elem_size = sizeof(new_elem);
        
        int insert_err = this->_set->insert(&new_elem, new_elem_size);
        if (insert_err != 0) {
            this->_set->clear();
            throw SetTesterInsertException(i, &new_elem, insert_err);
        }
    }

    auto start = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < this->_set->_set_data_len; i++) {
        size_t new_elem_size;
        Set::SetIterator* iter = dynamic_cast<Set::SetIterator*>(this->_set->find(&i, sizeof(i)));
        
        if (iter == nullptr) {
            this->_set->clear();
            throw SetTesterFindException(i, &i);
        }
        free(iter);
    }

    auto finish = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed = finish - start;
    std::cout << "Found " << this->_set->_set_data_len << " elements in " << elapsed.count() << " ms.\n";
    
    this->_set->clear();
}


void SetTester::remove_even_test() {
    for (size_t i = 0; i < this->_set->_set_data_len; i++) {
        size_t new_elem = i;
        size_t new_elem_size = sizeof(new_elem);
        
        int insert_err = this->_set->insert(&new_elem, new_elem_size);
        if (insert_err != 0) {
            this->_set->clear();
            throw SetTesterInsertException(i, &new_elem, insert_err);
        }
    }

    size_t* elem;
    size_t elem_size;
    size_t counter = 0;
    Set::SetIterator* set_iter = dynamic_cast<Set::SetIterator*>(this->_set->newIterator());
    
    auto start = std::chrono::high_resolution_clock::now();   
    do {
        elem = (size_t*)set_iter->getElement(elem_size);
        
        if (elem && *elem % 2 == 0) {
            this->_set->remove(set_iter);
            Set::Iterator* iter = this->_set->find(&elem, elem_size);
            if (!iter) {
                // std::cout << "removed " << *elem << "\n";
                counter++;
            } else {
                throw SetTesterRemoveException(elem);
            }
        } else {
            set_iter->goToNext();
        }
    } while (set_iter->hasNext());
    
    auto finish = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed = finish - start;
    std::cout << "Removed " << counter << " elements in " << elapsed.count() << " ms.\n";

    free(set_iter);
    this->_set->clear();
}

void SetTester::duplicate_iterator_test() {
    size_t elem = 1;
    int insert_err = this->_set->insert(&elem, sizeof(elem));
    if (insert_err != 0) {
        this->_set->clear();
        throw SetTesterInsertException(0, &elem, insert_err);
    }
    Set::Iterator* iter_1 = this->_set->newIterator();
    Set::Iterator* iter_2 = this->_set->find(&elem, sizeof(elem));

    this->_set->remove(iter_1);
    this->_set->remove(iter_2);

    free(iter_1);
    free(iter_2);
    this->_set->clear();
}

SetTester::~SetTester() {
    delete this->_set;
    delete this->_mem_manager;
    // вызывать деструктор менеджера памяти !!!
    // this->mem_manager->
}
