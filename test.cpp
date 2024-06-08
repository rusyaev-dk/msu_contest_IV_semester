#include <iostream>
#include "LinkedList1.hpp"
#include "./Mem.h"
#include <ctime>
#define MILL 1000000


using namespace std;



int main(){
    int max_memory = 100*MILL;
    Mem mem(max_memory);
    LinkedList test(mem);
    size_t size = 0;
    int var_k = 1;
    for(int i = 0; i < 1000 ; i++ ){
        test.push_front(&var_k , sizeof(var_k));
        var_k++;
    }
    Container::Iterator* new_iter = test.newIterator();
    cout << test.size() << endl;
    int value = 0;
    while(new_iter->hasNext()){
        value = *(int*)new_iter->getElement(size);
        if(value%2==0){
            test.remove(new_iter);
        }
        else{
            new_iter->goToNext();
            cout << value << endl;
        }
    }
    cout << test.size() << endl;
}