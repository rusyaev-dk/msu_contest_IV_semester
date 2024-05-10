#include "SetTesterExceptions.h"
#include <sstream>

SetTesterInsertException::SetTesterInsertException(void* elem, int err_code) : _elem(elem), _err_code(err_code) {}

int SetTesterInsertException::get_err_code() const {return this->_err_code;}

void* SetTesterInsertException::get_elem() const {return this->_elem;}

string SetTesterInsertException::what() const {
    std::stringstream ss;
    ss << "Couldn't insert elem at address: " << this->_elem << " Error code: " << this->_err_code;
    return ss.str();
}

// Find:

SetTesterFindException::SetTesterFindException(void* elem) : _elem(elem) {}

void* SetTesterFindException::get_elem() const {return this->_elem;}

string SetTesterFindException::what() const {
    std::stringstream ss;
    ss << "Couldn't find elem at address: " << this->_elem;
    return ss.str();
}

// Remove:

SetTesterRemoveException::SetTesterRemoveException(void* elem) : _elem(elem) {}

void* SetTesterRemoveException::get_elem() const {return this->_elem;}

string SetTesterRemoveException::what() const {
    std::stringstream ss;
    ss << "Couldn't insert elem at address: " << this->_elem;
    return ss.str();
}
