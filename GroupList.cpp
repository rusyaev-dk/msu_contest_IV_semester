#include "GroupList.h"

void* GroupList::ListIterator::getElement(size_t& size) {
    if (ptr && ptr->object) {
        size = ptr->obj_size;
        return ptr->object;
    }
    return NULL;
}

bool GroupList::ListIterator::hasNext() {
    if (ptr->next_ptr) return true;
    return false;
}

void GroupList::ListIterator::goToNext() {
    if (ptr) {
        ptr = ptr->next_ptr;
    } else {
        Error err("NULL ptr!");
        throw err;
    }
}

bool GroupList::ListIterator::equals(Iterator* right) {
    ListIterator* checking_iter = dynamic_cast<ListIterator*>(right);
    if (checking_iter) {
        if (ptr == NULL || checking_iter->ptr == NULL)
            return ptr == checking_iter->ptr;
        if (ptr->obj_size == checking_iter->ptr->obj_size &&
            !memcmp(ptr->object, checking_iter->ptr->object, ptr->obj_size))
            return true;
        else
            return false;
    } else {
        Error err("Dynamic cast error!");
        throw err;
    }
}

GroupList::Iterator* GroupList::newIterator() {
    if (list_head) {
        ListIterator* iter_ptr =
            (ListIterator*)GroupList::_memory.allocMem(sizeof(ListIterator));
        if (iter_ptr) {
            ListIterator new_iter(list_head);
            iter_ptr = (ListIterator*)memcpy(iter_ptr, &new_iter,
                                             sizeof(ListIterator));
            return iter_ptr;
        } else {
            Error err("Memory for iterator wasn`t allocated!");
            throw err;
        }
    }
    return NULL;
}

void GroupList::clear() {
    if (list_head) {
        ListElem* current_elem = list_head;
        ListElem* bufer;
        do {
            GroupList::_memory.freeMem(current_elem->object);
            bufer = current_elem;
            current_elem = current_elem->next_ptr;
            GroupList::_memory.freeMem(bufer);
        } while (current_elem);
        list_head = NULL;
    } else {
        Error err("Container is empty!");
        throw err;
    }
}

bool GroupList::empty() {
    if (list_head) return false;
    return true;
}

void* GroupList::group_list_front(size_t& size) {
    if (list_head->object) {
        size = list_head->obj_size;
        return list_head;
    }
    return NULL;
}