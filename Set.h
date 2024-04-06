#pragma once
#include "SetAbstract.h"
#include "LinkedList1.h"

using namespace std;

class Set : public AbstractSet {
private:
    LinkedList** _set_data;
    size_t _set_bytes_size;
    int _set_data_len;
    int _elem_count;

    Iterator* newIterator(int set_data_index);
    Iterator* newIterator(LinkedList::ListIterator* list_iterator, int set_data_index);

public:
    class SetIterator : public AbstractSet::Iterator {
    private:
        LinkedList::ListIterator* list_iterator;
        Set* set;
        int set_data_index;
        SetIterator(Iterator* iterator, Set* set, int set_data_index);
    public:
        void* getElement(size_t &size) override;
        bool hasNext() override;
        void goToNext() override;
        bool equals(Iterator *right) override;

        friend class Set;
    };

    Set(MemoryManager &mem);
    ~Set();

    int insert(void* elem, size_t size) override;

    // Функция возвращает значение, 
    // равное максимальной вместимости контейнера в байтах.
    size_t max_bytes() override;
    int size() override;

    Iterator* find(void* elem, size_t size) override;
    Iterator* newIterator() override;
    
    void remove(Iterator *iter) override;

    // Удаление всех элементов из контейнера.
    void clear() override;
    
    // Если контейнер пуст возвращает true, иначе false
    bool empty() override;
};