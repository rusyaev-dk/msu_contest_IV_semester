#include <iostream>
#include "LinkedList1.hpp"
#include <ctime>
using namespace std;



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

            for(int i = 0; i < num ; i++){
                test_list->push_front(&i , sizeof(i));
            }
            Container::Iterator* test_iter;
            for(int i = 0; i < num ; i++){
                test_iter = test_list->find(&i , sizeof(i));
                if(!test_iter){
                    cout << "Test failed!" << endl;
                    return 0;
                }
            }   
            cout << "Test succeed!" << endl;
            return 1;
        }

        int create_some_elements_2(int num){
            time_t start = clock();
            int memory = 10*num;
            Mem mem(memory);
            int new_elem;
            test_list = new List(mem);
            int* arr_num[num];
            for(int i = 0; i < num ; i++){
                test_list->push_front(&i , sizeof(i));
                arr_num[i] = &i;
            }
            Container::Iterator* test_iter = test_list->newIterator();
            size_t sizebf;
            for(int i = 0; i < num ; i++){
                if(arr_num[i]!=test_iter->getElement(sizebf)){
                    cout << "Test failed!" << endl;
                    return 0;
                }
                test_iter->goToNext();
            }   
            cout << "Test succeed!" << endl;
            return 1;
        }
        
        int delete_evens(int num){
            Container::Iterator* new_iter = test_list->newIterator();
            size_t size_bf;
            int buf;
            while(new_iter->hasNext()){
                buf = *(int*)new_iter->getElement(size_bf);
                if(buf%2==0){
                    test_list->remove(new_iter);
                }
                else{
                    new_iter->goToNext();
                }
            }
            
            new_iter = test_list->newIterator();

            for (int i = 0; i < test_list->size(); i++) {
                buf = *(int*)new_iter->getElement(size_bf);
                if (buf % 2 == 0) {
                    cout << "Test failed!" << endl;
                    return 1;
                }
            }

            for (int i = 0 ; i < test_list->size(); i++) {
                buf = *(int*)new_iter->getElement(size_bf);
                if (i % 2) {
                    if (buf != num - i) {
                        cout << "Test failed!" << endl;
                        return 1;
                    }
                    new_iter->goToNext();
                }
                
            }
            cout << "succeed" << endl;

            return 0;
        }

        

    
};

    
