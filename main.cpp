
#include <iostream>
#include "TableAbstract.h"
#include "Table.h"
#include "MainMemoryManager.h"
#include "List.h"

#define __CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#define DEBUG_NEW new_1(_NORMAL_BLOCK, FILE, LINE)
#define new_1 DEBUG_NEW


int main()
{
    //
    setlocale(LC_ALL, "Russian");
    
    MainMemoryManager memoryManager(10000000); // �����������, ��� � ��� ���� ���������� ������
    Table myTable(memoryManager);

    // ������������ ������� ���������
    const int numElements = 3;
    int successes = 0;
    for (int i = 1; i < numElements; ++i) {
        int key = i; 
        int value = i * 2; 

        if (myTable.insertByKey(&key, sizeof(int), &value, sizeof(int)) == 0) {
            ++successes;
        }
        else {
            std::cerr << "������ ��� ������� �������� � ������ " << key << std::endl;
        }
    }

    std::cout << "������� ��������� ���������: " << successes << " �� " << numElements-1 << std::endl;

   // // ������������ ������
    int keyToFind = 2; // ����, ������� �� ����� ������
    auto iter = myTable.findByKey(&keyToFind, sizeof(int));
    if (iter != nullptr) {
        std::cout << "������� � ������ " << keyToFind << " ������." << std::endl;
    }
    else {
        std::cerr << "������� � ������ " << keyToFind << " �� ������." << std::endl;
    }

    // ������������ ��������
    myTable.removeByKey(&keyToFind, sizeof(int));  // !!!!!!!!!!
     iter = myTable.findByKey(&keyToFind, sizeof(int));
    if (iter == nullptr) {
        std::cout << "������� � ������ " << keyToFind << " ������� ������." << std::endl;
    }
    else {
        std::cerr << "������ ��� �������� �������� � ������ " << keyToFind << std::endl;
    }

   // // ������������ ������� � ��������
   // int* keyToAccess = new int(2); // ����, � �������� �������� �� ����� �������� ������
   // size_t valueSize;
   // int* value = (int*)myTable.at(keyToAccess, sizeof(int), valueSize);
   // if (value != nullptr) {
   //     std::cout << "������ � �������� � ������ " << *keyToAccess << " �������, ��������: " << *value << std::endl;
   // }
   // else {
   //     std::cerr << "�� ������� �������� ������ � �������� � ������ " << *keyToAccess << std::endl;
   // }

   // // �����������, ��� ���� ���������� ���������� ������������ � ������������ ������
   // // ������� ���������� ������ (���� ��� ���������� � ����� ����������)
   // delete keyToFind;
   // delete keyToAccess;
    delete iter;
    myTable.clear();
    _CrtDumpMemoryLeaks();
    return 0;
    
    //*******************************************************************************************
    
   // MainMemoryManager memoryManager(100); 
   // Table* myTable = new Table(memoryManager);
   // size_t tableSize = 0;
   // 

   // int key = 20;
   // int elem = 2;
   // 

   // size_t keySize = sizeof(key);
   // size_t elemSize = sizeof(elem);
   // int hash_result = 0;
   // size_t valueSize = 20;
  

   // int result = myTable->insertByKey(&key, keySize, &elem, elemSize);
   // if (result == 0) {
   //     std::cout << "������� ������� ��������.\n";
   // }
   // else {
   //     std::cout << "������ ��� ������� ��������.\n";
   // }
   // 
   // /*
   // myTable->at(keyPtr, keySize, valueSize);

   // myTable->removeByKey(keyPtr, keySize);*/

   // Table::Iterator* iter = myTable->findByKey(&key, keySize);
   // 
   // if (iter != nullptr) {
   //     std::cout << "������� ������.\n";
   // }
   // else {
   //     std::cout << "������� �� ������.\n";
   // }


   // //**************************************************
   // void* value = myTable->at(&key, keySize, valueSize);
   // 
 
   // int* new_elem = static_cast<int*>(value);
   // std::cout << "elem: " << *new_elem;

   // 
   //
   // //****************************************************
   // 

   // Table::Iterator* new_iter = myTable->find(&elem, elemSize);




   // myTable->remove(new_iter);

   //// hash_result = myTable->hash_function(keyPtr, keySize);
   // 
   // 
   // //printf("%zu", hash_result);
   // // long long tmp = (long long)hash_result;

   // std::cout << "���-�������� �����: " << hash_result << "\n";
   //// _CrtDumpMemoryLeaks();
   // return 0;
    
}


