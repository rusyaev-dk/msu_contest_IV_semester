#pragma once
#include "SetAbstract.h"
#include "LinkedList1.h"

using namespace std;

class Set : public AbstractSet {
private:
    LinkedList** set_data;
    size_t set_size;
    int set_data_len;
    int elem_count;

    Iterator* newIterator(int set_data_index);

public:
    class SetIterator : public AbstractSet::Iterator {
    private:
        LinkedList::ListIterator* list_iterator;
        Set* set;
        int set_data_index;
        SetIterator(Iterator* iterator, Set* set, int set_data_index);
    public:
        void* getElement(size_t &size);
        bool hasNext();
        void goToNext();
        bool equals(Iterator *right);

        friend class Set;
    };

    Set(MemoryManager &mem);
    ~Set();

    int insert(void* elem, size_t size) override;

    // Функция возвращает значение, 
    // равное максимальной вместимости контейнера в байтах.
    size_t max_bytes();
    int size();

    Iterator* find(void* elem, size_t size);
    Iterator* newIterator();
    
    void remove(Iterator *iter);

    // Удаление всех элементов из контейнера.
    void clear();
    
    // Если контейнер пуст возвращает true, иначе false
    bool empty();
};