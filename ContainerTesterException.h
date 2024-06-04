#pragma once

#include <iostream>
#include <string>

using namespace std;

class ContainerTesterException {
public:
    enum ErrorCode {
        DUPLICATE_INSERT_ERROR,
        INSERT_ERROR,
        ELEM_NOT_FOUND_ERROR,
        REMOVE_ERROR,
        CLEAR_ERROR,
        ITERATOR_ERROR,
        DATA_ERROR,
        UNKNOWN_ERROR,
    };

private:
    ErrorCode _err_code;
    string _msg;
    void* _elem;

public:

    ContainerTesterException(ErrorCode err_code);
    ContainerTesterException(ErrorCode err_code, void* elem);
    ContainerTesterException(ErrorCode err_code, string msg);
    ContainerTesterException(ErrorCode err_code, void* elem, string msg);

    string what() const;
};
