#include <iostream>
#include "LinkedList1.hpp"
#include "./Mem.h"
#include <ctime>
#define MILL 500
#include "TestClass.hpp"

using namespace std;



int main(){
    time_t start = clock();
    int memory = 10*MILL;
    Mem mem(memory);
    int new_elem;
    List created_list(mem);
    for(int i = 0; i < MILL ; i++){
        new_elem = i;
        created_list.push_front(&new_elem , sizeof(new_elem));
    }
    time_t stop = clock();
    float overall_time = (stop-start)*1.0/CLOCKS_PER_SEC;
    cout << "Time of creating: " <<  overall_time << " sec" << endl;
    cout << created_list.size() << endl;
    new_elem = 5;
    size_t buf;
    Container::Iterator* new_iter;
    for(int i = 0 ; i < MILL; i++ ){
        if(i%2){
            new_iter = created_list.find(&i , sizeof(i));
            if(new_iter){
                created_list.remove(new_iter);
            }
        }
    }
    cout << created_list.size() << endl;
}
