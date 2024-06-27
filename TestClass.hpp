
#include <iostream>
#include "LinkedList1.hpp"
#include <ctime>
using namespace std;



class ListTest{
    private:
    List* test_list;

    public:

        int create_some_random_elements(int num , int range){
            time_t start = clock();
            int memory = 10*num;
            Mem mem(memory);
            int new_elem;
            test_list = new List(mem);

            for(int i = 0 ; i < num ; i++){
                new_elem = i;
                test_list->push_front(&new_elem , sizeof(new_elem));
            }

            time_t stop = clock();
            float overall_time = (stop-start)*1.0/CLOCKS_PER_SEC;
            cout << test_list->size() << endl;
            cout << "Time of creating: " <<  overall_time << " sec" << endl;
        }


        int delete_evens(){
            time_t start = clock();
            cout << test_list->size() << endl;
            Container::Iterator* new_iter = test_list->newIterator();
            size_t size;
            int buf;
            int start_size = test_list->size();
            while(new_iter->hasNext()){
                buf = *(int*)new_iter->getElement(size);
                if(buf%2==0){
                    test_list->remove(new_iter);
                }
                else{
                    new_iter->goToNext();
                }
            }
            
            cout << "Elements deleted: " << start_size - test_list->size() << endl;

            time_t stop = clock();
            float overall_time = (stop-start)*1.0/CLOCKS_PER_SEC;
            cout << "Time of creating: " <<  overall_time << " sec" << endl;
        }

    
};