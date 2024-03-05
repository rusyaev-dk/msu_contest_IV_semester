#pragma once
#include "GroupContainer.h"

// Абстрактный класс: множество
class AbstractSet: public GroupContainer
{
public:
    // конструктор
    AbstractSet(MemoryManager &mem): GroupContainer(mem) {}

    // деструктор
    virtual ~AbstractSet() {}

    // Добавление элемента в контейнер в соответствующую позицию, в соответствии с выбором реализации множества. В случае успешного добавления функция возвращает значение 0, если такой элемент уже есть в контейнере - 1, во всех остальных случаях - 2.
    virtual int insert(void *elem, size_t size) = 0;
};
