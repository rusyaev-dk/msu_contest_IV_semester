#pragma once
#include "Container.h"

// Базовый класс для однонаправленного и двунаправленного списков
class GroupList: public Container
{
public:
    GroupList(MemoryManager &mem): Container(mem) {}
};
