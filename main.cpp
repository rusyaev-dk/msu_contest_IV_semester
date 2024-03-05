#include "Set.h"
#include "MainMemoryManager.h"
#include <iostream>

int main() {
    MainMemoryManager main_mem_mamanger(100);
    Set my_set(main_mem_mamanger);
    return 0;
}