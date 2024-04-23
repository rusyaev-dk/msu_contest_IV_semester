#pragma once
#include <iostream>
#include <string>

class SetTesterException {};

class SetTesterInsertException : public SetTesterException {
    private:
        int _err_code;
        size_t _index;
        void* _elem;
    public:
        SetTesterInsertException(size_t index = 0, void* elem, int err_code = 0);

        int err_code();
        size_t index();
        void* elem();
};

class SetTesterFindException : public SetTesterException {
    private:
        size_t _index;
        void* _elem;
    public:
        SetTesterFindException(size_t index, void* elem);
        
        size_t index();
        void* elem();
};

class SetTesterRemoveException : public SetTesterException {
    private:
        void* _elem;
    public:
        SetTesterRemoveException(void* elem);
        
        void* elem();
};
