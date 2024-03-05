#include <iostream>
#include "LinkedList1.h"
using namespace std;

void* LinkedList::ListIterator::getElement(size_t &size){

}

LinkedList::LinkedList(MainMemoryManager &mem) : AbstractList(mem){
   this -> _memory.allocMem(sizeof(ListNode*));

}