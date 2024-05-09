#pragma once
#include <iostream>
#include <string>

using namespace std;

class SetTesterException {};

class SetTesterInsertException : public SetTesterException {
private:
    int _err_code;
    void* _elem;
public:
    SetTesterInsertException(void* elem, int err_code);

    int get_err_code();
    void* get_elem();
};

class SetTesterFindException : public SetTesterException {
private:
    void* _elem;
public:
    SetTesterFindException(void* elem);
    
    void* get_elem();
};

class SetTesterRemoveException : public SetTesterException {
private:
    void* _elem;
public:
    SetTesterRemoveException(void* elem);
    
    void* get_elem();
};
