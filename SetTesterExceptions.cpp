#include "SetTesterExceptions.h"

SetTesterInsertException::SetTesterInsertException(size_t index, void* elem, int err_code) :  _index(index), _elem(elem), _err_code(err_code) {}

int SetTesterInsertException::get_err_code() {return this->_err_code;}
size_t SetTesterInsertException::get_index() {return this->_index;}
void* SetTesterInsertException::get_elem() {return this->_elem;}


SetTesterFindException::SetTesterFindException(size_t index, void* elem) : _index(index), _elem(elem) {}

size_t SetTesterFindException::get_index() {return this->_index;}
void* SetTesterFindException::get_elem() {return this->_elem;}


SetTesterRemoveException::SetTesterRemoveException(void* elem) : _elem(elem) {}

void* SetTesterRemoveException::get_elem() {return this->_elem;}
