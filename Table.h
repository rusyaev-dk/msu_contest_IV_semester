#pragma once
#include "TableAbstract.h"
#include <iostream>
#include "MainMemoryManager.h"
#include "List.h"
#include "MemoryManager.h"


struct Node {
    void* key;
    size_t keySize; 
    void* elem;
    size_t elemSize; 
};



// ����������� �����: ������������ �������
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
       

    public:
        TableIterator(Table* table, int index, Iterator* iter);

        void* getElement(size_t& size);
        bool hasNext();
        void goToNext();
        bool equals(Iterator* right);
        friend class Table;
    };

    // �����������
      Table(MemoryManager& mem);

    // ���������� �������� � ���������, � �������������� ������.
    // ���� ����� ���� ��� ����, ������� ������ �� ������ � ���������� 1.
    // � ������ ��������� ���������� ������� ���������� �������� 0, � ������ ������� 1.
    int insertByKey(void* key, size_t keySize, void* elem, size_t elemSize) ;

    // �������� �������� � �������������� ������ �� ����������.
    void removeByKey(void* key, size_t keySize);

    // ������� ���������� ��������� �� ��������, ����������� �� ��������� � ���������� ������� � �������������� ������.
    // ���� ������� �� ������, ������������ ������� ���������.
    Iterator* findByKey(void* key, size_t keySize);

    // ������ � �������� � ������ key
     void* at(void* key, size_t keySize, size_t& valueSize);

    // ��� �������
    size_t hash_function(void* key, size_t keySize);

    size_t max_bytes();

    int size();

    Iterator* find(void* elem, size_t size);
    Iterator* newIterator();

    void remove(Iterator* iter);
    void clear();
    bool empty();

   // void rehashing()
    
    // ����������
    ~Table();
      

};

