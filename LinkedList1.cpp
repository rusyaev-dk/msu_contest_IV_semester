#include <iostream>
#include "LinkedList1.hpp"


using namespace std;




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
    ListNode* new_node = new ListNode(elem , head , elemSize);
    this->head = new_node;
    num_of_elems++;
 }


 void LinkedList::pop_front(){
    if(head->get_next()){
        ListNode* buf = head;
        head = head->get_next();
        delete buf;
    }
    else{
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
    num_of_elems++;
    if(list_iter->prev_node){
        ListNode* new_elem = new ListNode(elem , list_iter->cur_node , elemSize );
        list_iter->prev_node->change_next(new_elem);
        list_iter->cur_node = new_elem;
    }
    else{
        push_front(elem , elemSize);
    }
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
    ListNode* buf = remove_iter->cur_node;
    remove_iter->prev_node->change_next(remove_iter->cur_node->get_next());
    remove_iter->cur_node = remove_iter->cur_node->get_next();
    delete buf;
}


void LinkedList::clear(){
    while(num_of_elems--){
        pop_front();
    }
}

bool LinkedList::empty(){
    return (bool)(num_of_elems);
}



