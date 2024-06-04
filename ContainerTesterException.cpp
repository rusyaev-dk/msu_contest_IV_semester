#include "ContainerTesterException.h"

#include <sstream>

ContainerTesterException::ContainerTesterException(ErrorCode err_code): _err_code(err_code), _elem(nullptr), _msg("") {}
ContainerTesterException::ContainerTesterException(ErrorCode err_code, void* elem): _err_code(err_code), _elem(elem), _msg("") {}
ContainerTesterException::ContainerTesterException(ErrorCode err_code, string msg): _err_code(err_code), _elem(nullptr), _msg(msg) {}
ContainerTesterException::ContainerTesterException(ErrorCode err_code, void* elem, string msg): _err_code(err_code), _elem(elem), _msg(msg) {}

string ContainerTesterException::what() const {
    std::stringstream ss;
    
    switch (this->_err_code) {
        case DUPLICATE_INSERT_ERROR:
            ss << "Insert duplicate is not allowed.";
            break;
        case INSERT_ERROR:
            ss << "Elem was not inserted";
            break;
        case ELEM_NOT_FOUND_ERROR:
            ss << "Elem not found.";
            break;
        case REMOVE_ERROR:
            ss << "Elem was not removed.";
            break;
        case CLEAR_ERROR:
            ss << "Set was not cleared properly.";
            break;
        case ITERATOR_ERROR:
            ss << "Something wrong with the iterator.";
            break;
        case DATA_ERROR:
            ss << "Set data error.";
            break;   
        default:
            ss << "Unknown error.";    
    }

    if (!this->_msg.empty()) {
        ss << " Msg: " << this->_msg;
    }

    return ss.str();
}