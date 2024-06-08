#include "List_1.h"

int List::size() {
    int counter = 0;
    if (list_head) {
        ListElem* current_object = list_head;
        do {
            counter++;
            current_object = current_object->next_ptr;
        } while (current_object);
    }
    return counter;
}

size_t List::max_bytes() { return 0; }

GroupList::Iterator* List::find(void* elem, size_t size) {
    if (list_head && elem && size > 0) {
        ListIterator* iter_ptr =
            (ListIterator*)List::_memory.allocMem(sizeof(ListIterator));
        if (iter_ptr) {
            ListElem* current_elem = list_head;
            do {
                if (size == current_elem->obj_size &&
                    !memcmp(elem, current_elem->object, size)) {
                    ListIterator new_iter(current_elem);
                    ListIterator* new_iter_ptr = &new_iter;
                    iter_ptr = (ListIterator*)memcpy(iter_ptr, new_iter_ptr,
                                                     sizeof(ListIterator));
                    return iter_ptr;
                }
                current_elem = current_elem->next_ptr;
            } while (current_elem);
            _memory.freeMem(iter_ptr);
        } else {
            Error err("Memory for iterator wasn`t allocated!");
            throw err;
        }
    }
    return NULL;
}

void List::remove(Iterator* iter) {
    ListIterator* iterator = dynamic_cast<ListIterator*>(iter);
    if (iterator) {
        Iterator* bufer = List::newIterator();
        ListIterator* current_elem = dynamic_cast<ListIterator*>(bufer);
        if (current_elem) {
            if (iterator->equals(current_elem)) {
                iterator->goToNext();
                pop_front();
            } else {
                bufer = List::newIterator();
                ListIterator* previous_elem =
                    dynamic_cast<ListIterator*>(bufer);
                if (previous_elem) {
                    current_elem->goToNext();
                    do {
                        if (current_elem->equals(iterator)) {
                            List::_memory.freeMem(current_elem->ptr->object);
                            iterator->goToNext();
                            previous_elem->ptr->next_ptr = iterator->ptr;
                            List::_memory.freeMem(current_elem->ptr);
                            break;
                        } else {
                            previous_elem->goToNext();
                            current_elem->goToNext();
                        }
                    } while (current_elem->ptr);
                    List::_memory.freeMem(previous_elem);
                } else {
                    Error err("Dynamic cast error!");
                    throw err;
                }
            }
            List::_memory.freeMem(current_elem);
        } else {
            Error err("Dynamic cast error!");
            throw err;
        }
    } else {
        Error err("Dynamic cast error!");
        throw err;
    }
}

int List::push_front(void* elem, size_t elemSize) {
    if (!list_head)  // ���� ��������� ����
    {
        if (elemSize > 0 && elem) {
            list_head = (ListElem*)List::_memory.allocMem(sizeof(ListElem));
            if (list_head) {
                list_head->object = List::_memory.allocMem(elemSize);
                if (list_head->object) {
                    list_head->object =
                        memcpy(list_head->object, elem, elemSize);
                    list_head->next_ptr = NULL;
                    list_head->obj_size = elemSize;
                    return 0;
                }
            }
        }
    } else  // ���� � ��� ���� ��������
    {
        ListElem* new_node =
            (ListElem*)List::_memory.allocMem(sizeof(ListElem));
        if (new_node) {
            new_node->object = List::_memory.allocMem(elemSize);
            if (new_node->object && elemSize > 0 && elem) {
                new_node->object = memcpy(new_node->object, elem, elemSize);
                new_node->next_ptr = list_head;
                new_node->obj_size = elemSize;
                list_head = new_node;
                new_node = NULL;
                return 0;
            }
        }
    }
    return 1;
}

void List::pop_front() {
    if (list_head) {
        List::_memory.freeMem(list_head->object);
        ListElem* bufer;
        bufer = list_head->next_ptr;
        List::_memory.freeMem(list_head);
        list_head = bufer;
        bufer = NULL;
    } else {
        Error err("Container is empty!");
        throw err;
    }
}

void* List::front(size_t& size) { return group_list_front(size); }

int List::insert(Iterator* iter, void* elem, size_t elemSize) {
    ListIterator* iterator = dynamic_cast<ListIterator*>(iter);
    if (iterator) {
        if (iterator->ptr) {
            Iterator* bufer = List::newIterator();
            ListIterator* current_elem = dynamic_cast<ListIterator*>(bufer);
            if (current_elem) {
                if (iterator->equals(
                        current_elem))  // ���� ��������� � ������ ������
                {
                    return push_front(elem, elemSize);
                } else  // ���� ��������� �� � ������
                {
                    bufer = List::newIterator();
                    ListIterator* previous_elem =
                        dynamic_cast<ListIterator*>(bufer);
                    if (previous_elem) {
                        current_elem->goToNext();
                        do {
                            if (current_elem->equals(iterator)) {
                                ListElem* new_node =
                                    (ListElem*)List::_memory.allocMem(
                                        sizeof(ListElem));
                                if (new_node && elemSize > 0 && elem) {
                                    new_node->object =
                                        List::_memory.allocMem(elemSize);
                                    if (new_node->object) {
                                        new_node->object = memcpy(
                                            new_node->object, elem, elemSize);
                                        new_node->obj_size = elemSize;
                                        new_node->next_ptr = current_elem->ptr;
                                        previous_elem->ptr->next_ptr = new_node;
                                        List::_memory.freeMem(current_elem);
                                        List::_memory.freeMem(previous_elem);
                                        return 0;
                                    } else {
                                        List::_memory.freeMem(previous_elem);
                                        List::_memory.freeMem(current_elem);
                                        return 1;
                                    }
                                } else {
                                    List::_memory.freeMem(previous_elem);
                                    List::_memory.freeMem(current_elem);
                                    return 1;
                                }
                            } else {
                                previous_elem->goToNext();
                                current_elem->goToNext();
                            }
                        } while (current_elem->ptr);
                        List::_memory.freeMem(previous_elem);
                    }
                }
                List::_memory.freeMem(current_elem);
            }
        }
        return 1;
    } else {
        return 1;
    }
}