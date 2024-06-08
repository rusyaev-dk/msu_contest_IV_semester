#pragma once
#include "TableAbstract.h"
#include <iostream>
#include "Mem.h"
#include "List.h"
#include "MemoryManager.h"


struct Node {
    void* key;
    size_t keySize; 
    void* elem;
    size_t elemSize; 
};


// Абстрактный класс: ассциативная таблица
class Table :public AbstractTable
{
private:
    
    List** tableMemory;
    size_t tableSize;
    int obj_count;
    
public:
    
   
    class TableIterator : public AbstractTable::Iterator {
        List::ListIterator* list_iterator;
        Table* table;
        int index;
       
        size_t getIndex();
    public:
        TableIterator(Table* table, int index, Iterator* iter);

        void* getElement(size_t& size);
        bool hasNext();
        void goToNext();
        bool equals(Iterator* right);
        friend class Table;
        ~TableIterator() {
            if (list_iterator) { table->_memory.freeMem(list_iterator); }
        }
    };

    friend class TableTest;
    // конструктор
      Table(MemoryManager& mem);

    // Добавление элемента в контейнер, с сответствующим ключом.
    // Если такой ключ уже есть, функция ничего не делает и возвращает 1.
    // В случае успешного добавления функция возвращает значение 0, в случае неудачи 1.
    int insertByKey(void* key, size_t keySize, void* elem, size_t elemSize) ;

    // Удаление элемента с сответствующим ключом из контейнера.
    void removeByKey(void* key, size_t keySize);

    // Функция возвращает указатель на итератор, указывающий на найденный в контейнере элемент с сответствующим ключом.
    // Если элемент не найден, возвращается нулевой указатель.
    Iterator* findByKey(void* key, size_t keySize);

    // доступ к элементу с ключом key
     void* at(void* key, size_t keySize, size_t& valueSize);

    // хэш функция
     size_t hash_function(void* key, size_t keySize);
        

    size_t max_bytes();

    int size();

    Iterator* find(void* elem, size_t size);
    Iterator* newIterator();

    void remove(Iterator* iter);
    void clear();
    bool empty();

    void rehashing();
    
    void printTable();
    // деструктор
   
    ~Table();
      
    

};

