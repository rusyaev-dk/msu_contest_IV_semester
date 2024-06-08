
#include <iostream>
#include "LinkedList1.hpp"
 

using namespace std;

LinkedList::~LinkedList(){
    this->clear();  
}
// LinkedList::LinkedList(MemoryManager &mem) : AbstractList(mem){
// //     LinkedList* new_list = (LinkedList*)_memory.allocMem(sizeof(LinkedList));
// //     new_list->head = NULL;
// //     new_list->num_of_elems = 0;
//        this->head = NULL;
//         this->num_of_elems = 0;
//  }

void* LinkedList::ListIterator::getElement(size_t &size){
    size = sizeof(this->cur_node->get_data());
    return this->cur_node->get_data();
}

bool LinkedList::ListIterator::hasNext(){
    return (bool)(this->cur_node->get_next());
}

void LinkedList::ListIterator::goToNext(){
    if(this->hasNext()){
        this->prev_node = cur_node;
        this->cur_node = this->cur_node->get_next();
    }
}

 bool LinkedList::ListIterator::equals(Iterator *right){
    ListIterator* list_right = dynamic_cast<ListIterator*>(right);
    return (bool)(this==list_right);
 }


 int LinkedList::push_front(void *elem, size_t elemSize){
        void* new_data = _memory.allocMem(sizeof(elem));
        memcpy(new_data , elem , sizeof(elem));
        ListNode* new_node = new ListNode(new_data, head , elemSize);
        this->head = new_node;
        num_of_elems++;
        return 0;
 }


 void LinkedList::pop_front(){
    if(head->get_next()){
        ListNode* buf = head;
        head = head->get_next();
        _memory.freeMem(buf->get_data());
        delete buf;
    }
    else{
        _memory.freeMem(head->get_data());
       delete head;
    }
    num_of_elems--;
 }


void* LinkedList::front(size_t &size){
    size = head->get_size();
    return head->get_data();
}


int LinkedList::insert(Iterator *iter, void *elem, size_t elemSize){
    ListIterator* list_iter = dynamic_cast<ListIterator*>(iter);
    if(list_iter->prev_node){
        num_of_elems++;
        void* new_data = _memory.allocMem(sizeof(elem));
        memcpy(new_data , elem , sizeof(elem));
        ListNode* new_elem = new ListNode(new_data , list_iter->cur_node , elemSize );
        list_iter->prev_node->change_next(new_elem);
        list_iter->cur_node = new_elem;
    }
    else{
        push_front(elem , elemSize);
    }
    return 0;
}



size_t LinkedList::max_bytes(){
    return 0;
}


int LinkedList::size(){
    return num_of_elems;
}


Container::Iterator* LinkedList::newIterator(){
    return new ListIterator(head , NULL);
}

Container::Iterator* LinkedList::find(void *elem, size_t size){
    ListIterator* find_iter = dynamic_cast<ListIterator*>(newIterator());
    while(find_iter->hasNext()){
        if(find_iter->cur_node->get_data() == elem){
            return find_iter;
        }
        find_iter->goToNext();
    }
    return NULL;
}




void LinkedList::remove(Iterator* iter){
    ListIterator* remove_iter = dynamic_cast<ListIterator*>(iter);
    if(remove_iter->prev_node==NULL){
        remove_iter->goToNext();
        pop_front();
    }
    else{
        ListNode* buf = remove_iter->cur_node;
        remove_iter->prev_node->change_next(remove_iter->cur_node->get_next());
        remove_iter->cur_node = remove_iter->cur_node->get_next();
        _memory.freeMem(buf->get_data());
        num_of_elems--;
        delete buf;
    }
}


void LinkedList::clear(){
    while(num_of_elems!=0){
        pop_front();
    }
}

bool LinkedList::empty(){
    return !(bool)(num_of_elems);
}



