#include "SetTesterExceptions.h"
#include <sstream>

SetTesterException::SetTesterException(ErrorCode err_code): _err_code(err_code), _elem(nullptr), _msg("") {}
SetTesterException::SetTesterException(ErrorCode err_code, void* elem): _err_code(err_code), _elem(elem), _msg("") {}
SetTesterException::SetTesterException(ErrorCode err_code, string msg): _err_code(err_code), _elem(nullptr), _msg(msg) {}
SetTesterException::SetTesterException(ErrorCode err_code, void* elem, string msg): _err_code(err_code), _elem(elem), _msg(msg) {}

string SetTesterException::what() const {
    std::stringstream ss;
    
    switch (this->_err_code) {
        case DUPLICATE_INSERT_ERROR:
            ss << "Insert duplicate is not allowed.";
            break;
        case ELEM_NOT_FOUND_ERROR:
            ss << "Elem not found.";
            break;
        case REMOVE_ERROR:
            ss << "Couldn't remove elem.";
            break;
        default:
            ss << "Unknown error.";
        
        if (this->_msg.size() > 1) {
            ss << " Msg: " << this->_msg;
        }
        
    }
    
    return ss.str();
}
