#pragma once

#include <iostream>
#include <string>

using namespace std;

class SetTesterException {
public:
    enum ErrorCode {
        DUPLICATE_INSERT_ERROR,
        INSERT_ERROR,
        ELEM_NOT_FOUND_ERROR,
        REMOVE_ERROR,
        CLEAR_ERROR,
        ITERATOR_ERROR,
        UNKNOWN_ERROR,
    };

private:
    ErrorCode _err_code;
    string _msg;
    void* _elem;

public:

    SetTesterException(ErrorCode err_code);
    SetTesterException(ErrorCode err_code, void* elem);
    SetTesterException(ErrorCode err_code, string msg);
    SetTesterException(ErrorCode err_code, void* elem, string msg);

    string what() const;
};
