#pragma once
#include "SetAbstract.h"
#include "LinkedList1.h"

using namespace std;

class Set : public AbstractSet {
private:
    LinkedList** set_data;
    int set_size;
public:
    class SetIterator : public AbstractSet::Iterator {
        LinkedList::ListIterator* list_iterator;
        Set* set;
        int set_data_index;
        
        public:
            SetIterator(Iterator* iterator, Set* set, int set_data_index);

            void* getElement(size_t &size);
            bool hasNext();
            void goToNext();
            bool equals(Iterator *right);
    };

    Set(MemoryManager &mem);

    int insert(void* elem, size_t size) override;

    size_t max_bytes() {};
    int size();

    Iterator* find(void* elem, size_t size) {};
    Iterator* newIterator();

    void remove(Iterator *iter) {};
    void clear() {};
    bool empty() {};
    
    ~Set();
};