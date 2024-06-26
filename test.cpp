#include <iostream>
#include "LinkedList1.hpp"
#include "./Mem.h"
#include <ctime>
#define MILL 1000000


using namespace std;



int main(){
    int max_memory = 100*MILL;
    Mem mem(max_memory);
    List test(mem);
    for(int i = 0 ; i < MILL; i++){
        test.push_front(&i , sizeof(i));
    }
    Container::Iterator* new_iter = test.newIterator();

    cout << test.size() << endl;

    while(new_iter->hasNext()){
        test.remove(new_iter);
    }
    cout << test.size() << endl;
}