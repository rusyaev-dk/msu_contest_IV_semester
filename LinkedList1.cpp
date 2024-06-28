
#include <iostream>
#include "LinkedList1.hpp"
 

using namespace std;

List::~List(){
    this->clear();  
}

 int List::push_front(void *elem, size_t elemSize){
        void* new_data = _memory.allocMem(elemSize);
        memcpy(new_data , elem , elemSize);
        ListNode* new_node = new ListNode(new_data, head , elemSize);
        this->head = new_node;
        num_of_elems++;
        return 0;
 }


 void List::pop_front(){
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


void* List::front(size_t &size){
    size = head->get_size();
    return head->get_data();
}

// int List::insert(Iterator *iter, void *elem, size_t elemSize){
//     ListIterator* list_iter = dynamic_cast<ListIterator*>(iter);

// }


int List::insert(Iterator *iter, void *elem, size_t elemSize){
    ListIterator* list_iter = dynamic_cast<ListIterator*>(iter);
    if(list_iter->prev_node){
        num_of_elems++;
        void* new_data = _memory.allocMem(elemSize);
        memcpy(new_data , elem , elemSize);
        ListNode* new_elem = new ListNode(new_data , list_iter->cur_node , elemSize );
        list_iter->prev_node->change_next(new_elem);
        list_iter->cur_node = new_elem;
    }
    else{
        push_front(elem , elemSize);
        list_iter->cur_node = head;
    }
    return 0;
}



size_t List::max_bytes(){
    return 0;
}


int List::size(){
    return num_of_elems;
}

Container::Iterator* List::newIterator(){
    if(this->head){
        Iterator* new_iter = new ListIterator(head , NULL);
        return new_iter;
    }
    else{
        cout << "container is empty" << endl;
        return NULL;
    }
}


Container::Iterator* List::find(void *elem, size_t size){
    Iterator* find_iter = this->newIterator(); 
    size_t size_bf;
    while(find_iter->hasNext()){
        if(memcmp(find_iter->getElement(size_bf) , elem , size)==0){
            return find_iter;
        }
        find_iter->goToNext();
    }
    cout << "ne nawel" << endl;
    return NULL;
}


void List::remove(Iterator* iter){
    ListIterator* remove_iter = dynamic_cast<ListIterator*>(iter);
    if(remove_iter->hasNext()){
        if(remove_iter->prev_node==NULL){
            remove_iter->cur_node = remove_iter->cur_node->get_next();
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
    else{
        _memory.freeMem(remove_iter->cur_node);
        remove_iter->cur_node = NULL;
        num_of_elems--;
    }
}

void List::clear(){
    while(num_of_elems!=0){
        pop_front();
    }
}

bool List::empty(){
    return !(bool)(num_of_elems);
}



