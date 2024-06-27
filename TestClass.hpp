
#include <iostream>
#include "LinkedList1.hpp"
#include <ctime>
using namespace std;



class ListTest{
    public:
    List* create_random_elements(int num){
        time_t start = clock();
        int memory = 10*num;
        Mem mem(memory);
        int new_elem;
        List created_list(mem);
        for(int i = 0; i < num ; i++){
            new_elem = 5;
            created_list.push_front(&new_elem , sizeof(new_elem));
        }
        time_t stop = clock();
        float overall_time = (stop-start)*1.0/CLOCKS_PER_SEC;
        cout << "Time of creating: " <<  overall_time << " sec" << endl;
        return &created_list;
    }  
};