#include <iostream>
#include "LinkedList1.hpp"
#include "./Mem.h"
#include <ctime>
#define MILL 100000
#include "TestClass.hpp"

using namespace std;



int main(){

    ListTest* new_test = new ListTest();
    new_test->create_some_elements(1000000 , 10);
    new_test->delete_evens();
}
