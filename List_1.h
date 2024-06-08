#pragma once
#include"ListAbstract.h"
#include"Mem.h"

class List : public AbstractList
{
public:
	List(MemoryManager& mem) : AbstractList(mem) { list_head = NULL; }
	~List() { if (list_head) List::clear(); }

	int size();
	size_t max_bytes(); // to be, or not to be?
	Iterator* find(void* elem, size_t size); 
	void remove(Iterator* iter);

	int push_front(void* elem, size_t elemSize);
	void pop_front();
	void* front(size_t& size);
	int insert(Iterator* iter, void* elem, size_t elemSize);
};