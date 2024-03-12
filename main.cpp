
#include "MainMemoryManager.h"
#include "Set.h"
#include <iostream>

int main() {
    MainMemoryManager main_mem_mamanger(100);
    Set my_set(main_mem_mamanger);

    int my_elem = 3243232;

    int err = my_set.insert(&my_elem, sizeof(my_elem));
    if (err != 0) std::cout << "Error: " << err << std::endl;
    
    Set::Iterator* set_iterator = my_set.newIterator();
    
    // std::cout << "_____";

    size_t my_elem_size;
    int* my_saved_elem = (int*)set_iterator->getElement(my_elem_size);

    std::cout << "My elem: " << *my_saved_elem << std::endl;
    return 0;
}