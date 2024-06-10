#ifndef LIST_H
#define LIST_H
#include <iostream>
#include "ListAbstract.h"
#include "Mem.h"

using namespace std;



class ListNode{
    private:
    void* data;
    ListNode* next;
    size_t elem_size;
    public:
        ListNode(void* new_data , ListNode* new_next , size_t new_size){
            this->data = new_data;
            this->next = new_next;
            this->elem_size = new_size;
        }
        ~ListNode() = default;

    void* get_data(){
        return this->data;
    }

    size_t get_size(){
        return this->elem_size;
    }

    ListNode* get_next(){ 
        return this->next;
    }

    void change_next(ListNode* new_next){
        this->next = new_next;
    }
};



class List:public AbstractList{
    private:
    ListNode* head;
    int num_of_elems;

    public:




    class ListIterator : public Container::Iterator{
        private:
        ListNode* cur_node;
        ListNode* prev_node;
        public:
        ListIterator(ListNode* new_cur , ListNode* new_prew){
            this->cur_node = new_cur;
            this->prev_node = new_prew;
        }
        // Возврашает явно указатель на элемент, на который указывает итератор в данный момент.
        // Неявно возвращает размер данных.
        // Если итератор показывает за пределы контейнера (например, удален последний элемент), возвращает NULL.
        void* getElement(size_t &size);

        // Возвращает true, если есть следующий элемент, иначе false.
        bool hasNext();

        // Переход к следующему элементу.
        void goToNext();

        // проверка на равенство итераторов
        bool equals(Iterator *right);

        friend class ListNode;
        friend class List;
    };



    List(MemoryManager &mem) : AbstractList(mem){
        head = NULL;
        num_of_elems = 0;
    }



    ~List();
    // Добавление элемента в начало контейнера.
    // В случае успешного добавления функция возвращает значение 0, в случае неудачи 1.
    int push_front(void *elem, size_t elemSize);

    // Удаление элемента из начала контейнера.
    void pop_front();

    // Функция явно возвращает указатель на элемент, находящийся в начале контейнера.
    // и не явно возвращает размер данных
    void* front(size_t &size);

    // Добавление элемента в позицию, на которую указывает итератор iter.
    // В случае успешного добавления функция возвращает значение 0, в случае неудачи 1.
    int insert(Iterator *iter, void *elem, size_t elemSize);

        // Функция возвращает значение, равное количеству элементов в контейнере.
    int size();

    // Функция возвращает значение, равное максимальной вместимости контейнера в байтах.
    size_t max_bytes();

    // Функция создает в динамической памяти итератор, указывающий на первый найденный
    // в контейнере элемент. Если элемент не найден, возвращается пустой указатель.
    // Удаление этого итератора должно делаться пользователем с помощью оператора delete.
    Iterator* find(void *elem, size_t size);

    // Функция создает в динамической памяти итератор, указывающий на первый элемент
    // контейнера. Если контейнер пустой, возвращается нулевой указатель.
    // Удаление этого итератора должно делаться пользователем с помощью оператора delete.
    Iterator* newIterator();

    // Удаление элемента из позиции, на которую указывает итератор iter.
    // После удаления итератор указывает на следующий за удаленным элемент.
    void remove(Iterator *iter);

    // Удаление всех элементов из контейнера.
    void clear();

    // Если контейнер пуст возвращает true, иначе false
    bool empty();
};

#endif