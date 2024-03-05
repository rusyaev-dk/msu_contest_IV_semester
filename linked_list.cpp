#include "LinkedList.h"

LinkedList::LinkedList() : head(nullptr), size(0) {}

LinkedList::~LinkedList() {
    while (head != nullptr) {
        ListNode* temp = head;
        head = head->next;
        delete temp;
    }
}

void LinkedList::append(void* value) {
    ListNode* newNode = new ListNode(value);
    if (head == nullptr) {
        head = newNode;
    } else {
        ListNode* current = head;
        while (current->next != nullptr) {
            current = current->next;
        }
        current->next = newNode;
    }
    size++;
}

void LinkedList::prepend(void* value) {
    ListNode* newNode = new ListNode(value);
    newNode->next = head;
    head = newNode;
    size++;
}

void LinkedList::remove(void* value) {
    if (head == nullptr) return;

    if (head->data == value) {
        ListNode* temp = head;
        head = head->next;
        delete temp;
        size--;
        return;
    }

    ListNode* current = head;
    while (current->next != nullptr) {
        if (current->next->data == value) {
            ListNode* temp = current->next;
            current->next = current->next->next;
            delete temp;
            size--;
            return;
        }
        current = current->next;
    }
}

void LinkedList::print() const {
    ListNode* current = head;
    while (current != nullptr) {
        std::cout << current->data << " ";
        current = current->next;
    }
    std::cout << std::endl;
}

int LinkedList::getSize() const {
    return size;
}
