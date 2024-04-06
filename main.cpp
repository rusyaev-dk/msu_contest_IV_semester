#include "MainMemoryManager.h"
#include "Set.h"
#include <iostream>

int main() {
    MainMemoryManager main_mem_mamanger(100);
    Set my_set(main_mem_mamanger);

    double my_elem = -1.54;
    size_t my_elem_size = sizeof(my_elem);

    // double my_elem_2 = 1.55;
    // size_t my_elem_size_2 = sizeof(my_elem_2);

    int err = my_set.insert(&my_elem, my_elem_size);
    // int err2 = my_set.insert(&my_elem_2, my_elem_size_2);
    // if (err != 0 || err2 != 0) std::cout << "Error: " << err << std::endl;
    
    Set::Iterator* set_iterator = my_set.find(&my_elem, my_elem_size);
    if (set_iterator) {
        size_t new_size;
        double found_elem = *(double*)(set_iterator->getElement(new_size));
        std::cout << "Found elem: " << found_elem << "\n";
    }

    // my_set.remove(set_iterator);
    
    // if (set_iterator) {
    //     size_t new_size;
    //     double found_elem = *(double*)(set_iterator->getElement(new_size));
    //     std::cout << "Found elem: " << found_elem << "\n";
    // }

    return 0;
}