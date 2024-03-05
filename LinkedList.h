#pragma once

#include <iostream>

struct ListNode {
    void* data;
    ListNode* next;

    ListNode(void* value) : data(value), next(nullptr) {}
};

class LinkedList {
private:
    ListNode* head;
    int size;

public:
    LinkedList();
    ~LinkedList();

    void append(void* value);
    void prepend(void* value);
    void remove(void* value);
    void print() const;
    int getSize() const;
};
