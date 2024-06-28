
#include <iostream>
#include "LinkedList1.hpp"
#include <ctime>
using namespace std;

void JAMALOV_SP(int JAMALOV) {
    while (1) {
        printf("JAMALOC SPPP!!!!\n");
    }
}



class ListTest{
    private:
    List* test_list;

    public:
        // cоздает список и пушит num  элементов со значениями от 0 до num-1.
        int create_some_elements(int num){
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


        // cоздает список и пушит num  элементов со случайными значениями в диапазоне range.
        int create_some_elements(int num , int range){
            time_t start = clock();
            int memory = 100000;
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

            Container::Iterator* new_iter1 = test_list->newIterator();
            int i = 0;
            for(int n = 0 ; n < 10 ; n++){
                i++;
                test_list->insert(new_iter1 , &i , sizeof(i));
            }
            cout << test_list->size() << endl;
            
            cout << "Time of creating: " <<  overall_time << " sec" << endl;
            
            
        }

        //удаляет все четные элементы и проверяет их наличие в списке.В случае успешного теста выдает 1,иначе 0;
        int delete_evens(){
            time_t start = clock();
            cout << test_list->size() << endl;
            Container::Iterator* new_iter = test_list->newIterator();
            size_t size;
            int buf;
            int start_size = test_list->size();


            cout << "Elements deleted: " << start_size - test_list->size() << endl;

            time_t stop = clock();
            float overall_time = (stop-start)*1.0/CLOCKS_PER_SEC;
            cout << "Time of creating: " <<  overall_time << " sec" << endl;
        }

        int insert_test(){
            Container::Iterator* new_iter = test_list->newIterator();
            int i = 0;
            // while(new_iter->hasNext()){
                i++;
                test_list->insert(new_iter , &i , sizeof(i));
            // }
        }

    
};