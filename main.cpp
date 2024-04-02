#include "MainMemoryManager.h"
#include "Set.h"
#include <iostream>

int main() {
    MainMemoryManager main_mem_mamanger(100);
    Set my_set(main_mem_mamanger);

    int my_elem = 3346232;
    size_t my_elem_size = sizeof(my_elem);

    int err = my_set.insert(&my_elem, my_elem_size);
    if (err != 0) std::cout << "Error: " << err << std::endl;
    
    Set::Iterator* set_iterator = my_set.newIterator();
    
    Set::Iterator* set_iterator2 = my_set.find(&my_elem, my_elem_size);
    if (set_iterator2) {
        size_t new_size;
        std::cout << "Found!\n\n";
        std::cout << *(int*)(set_iterator2->getElement(new_size)) << "\n";
    } else {
        std::cout << "not Found!\n\n";
    }

    // std::cout << "_____";

    // size_t my_elem_size;
    // int* my_saved_elem = (int*)set_iterator->getElement(my_elem_size);

    // std::cout << "My elem: " << *my_saved_elem << std::endl;
    return 0;
}