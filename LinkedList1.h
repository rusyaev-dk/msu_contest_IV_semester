#include <iostream>
#include "ListAbstract.h"
#include "MainMemoryManager.h"
using namespace std; 



class LinkedList: public AbstractList {

    private:

    void* data;
    LinkedList* first_elem;
    LinkedList* next_elem;

    public:
    class ListIterator:public AbstractList::Iterator{
    public:
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
    };

    LinkedList(MainMemoryManager &mem);

    // деструктор
    ~LinkedList() {}

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


};