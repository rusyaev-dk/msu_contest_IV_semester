
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
    
    MainMemoryManager memoryManager(10000000); // Предположим, что у нас есть достаточно памяти
    Table myTable(memoryManager);

    // Тестирование вставки элементов
    const int numElements = 3;
    int successes = 0;
    for (int i = 1; i < numElements; ++i) {
        int key = i; 
        int value = i * 2; 

        if (myTable.insertByKey(&key, sizeof(int), &value, sizeof(int)) == 0) {
            ++successes;
        }
        else {
            std::cerr << "Ошибка при вставке элемента с ключом " << key << std::endl;
        }
    }

    std::cout << "Успешно вставлено элементов: " << successes << " из " << numElements-1 << std::endl;

   // // Тестирование поиска
    int keyToFind = 2; // Ключ, который мы будем искать
    auto iter = myTable.findByKey(&keyToFind, sizeof(int));
    if (iter != nullptr) {
        std::cout << "Элемент с ключом " << keyToFind << " найден." << std::endl;
    }
    else {
        std::cerr << "Элемент с ключом " << keyToFind << " не найден." << std::endl;
    }

    // Тестирование удаления
    myTable.removeByKey(&keyToFind, sizeof(int));  // !!!!!!!!!!
     iter = myTable.findByKey(&keyToFind, sizeof(int));
    if (iter == nullptr) {
        std::cout << "Элемент с ключом " << keyToFind << " успешно удален." << std::endl;
    }
    else {
        std::cerr << "Ошибка при удалении элемента с ключом " << keyToFind << std::endl;
    }

   // // Тестирование доступа к элементу
   // int* keyToAccess = new int(2); // Ключ, к элементу которого мы хотим получить доступ
   // size_t valueSize;
   // int* value = (int*)myTable.at(keyToAccess, sizeof(int), valueSize);
   // if (value != nullptr) {
   //     std::cout << "Доступ к элементу с ключом " << *keyToAccess << " получен, значение: " << *value << std::endl;
   // }
   // else {
   //     std::cerr << "Не удалось получить доступ к элементу с ключом " << *keyToAccess << std::endl;
   // }

   // // Предположим, что есть корректная реализация деструкторов и освобождения памяти
   // // Удаляем выделенную память (если это необходимо в вашей реализации)
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
   //     std::cout << "Элемент успешно вставлен.\n";
   // }
   // else {
   //     std::cout << "Ошибка при вставке элемента.\n";
   // }
   // 
   // /*
   // myTable->at(keyPtr, keySize, valueSize);

   // myTable->removeByKey(keyPtr, keySize);*/

   // Table::Iterator* iter = myTable->findByKey(&key, keySize);
   // 
   // if (iter != nullptr) {
   //     std::cout << "Элемент найден.\n";
   // }
   // else {
   //     std::cout << "Элемент не найден.\n";
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

   // std::cout << "Хэш-значение ключа: " << hash_result << "\n";
   //// _CrtDumpMemoryLeaks();
   // return 0;
    
}


