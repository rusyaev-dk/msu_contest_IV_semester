#pragma once
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include "MemoryManager.h"


using namespace std;

// Базовый класс для всех абстрактных контейнеров
class Container
{
protected:
    MemoryManager &_memory;
public:
    // Базовый класс для исключений, которые запускает контейнер
    struct Error
    {
        char msg[256];
        Error(const char *err_msg)
        {
            strcpy(msg, err_msg);
        }
    };

    class Iterator
    {
    public:
        // Возврашает явно указатель на элемент, на который указывает итератор в данный момент.
        // Неявно возвращает размер данных.
        // Если итератор показывает за пределы контейнера (например, удален последний элемент), возвращает NULL.
        virtual void* getElement(size_t &size) = 0;

        // Возвращает true, если есть следующий элемент, иначе false.
        virtual bool hasNext() = 0;

        // Переход к следующему элементу.
        virtual void goToNext() = 0;

        // проверка на равенство итераторов
        virtual bool equals(Iterator *right) = 0;
    };

    Container(MemoryManager &mem): _memory(mem) {}

    // Функция возвращает значение, равное количеству элементов в контейнере.
    virtual int size() = 0;

    // Функция возвращает значение, равное максимальной вместимости контейнера в байтах.
    virtual size_t max_bytes() = 0;

    // Функция создает в динамической памяти итератор, указывающий на первый найденный
    // в контейнере элемент. Если элемент не найден, возвращается пустой указатель.
    // Удаление этого итератора должно делаться пользователем с помощью оператора delete.
    virtual Iterator* find(void *elem, size_t size) = 0;

    // Функция создает в динамической памяти итератор, указывающий на первый элемент
    // контейнера. Если контейнер пустой, возвращается нулевой указатель.
    // Удаление этого итератора должно делаться пользователем с помощью оператора delete.
    virtual Iterator* newIterator() = 0;

    // Удаление элемента из позиции, на которую указывает итератор iter.
    // После удаления итератор указывает на следующий за удаленным элемент.
    virtual void remove(Iterator *iter) = 0;

    // Удаление всех элементов из контейнера.
    virtual void clear() = 0;

    // Если контейнер пуст возвращает true, иначе false
    virtual bool empty() = 0;
};
