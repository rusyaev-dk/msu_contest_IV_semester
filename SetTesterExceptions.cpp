#include "SetTesterExceptions.h"

SetTesterInsertException::SetTesterInsertException(size_t index, void* elem, int err_code) :  _index(index), _elem(elem), _err_code(err_code) {}

int SetTesterInsertException::err_code() {return this->_err_code;}
size_t SetTesterInsertException::index() {return this->_index;}
void* SetTesterInsertException::elem() {return this->_elem;}


SetTesterFindException::SetTesterFindException(size_t index, void* elem) : _index(index), _elem(elem) {}

size_t SetTesterFindException::index() {return this->_index;}
void* SetTesterFindException::elem() {return this->_elem;}


SetTesterRemoveException::SetTesterRemoveException(void* elem) : _elem(elem) {}

void* SetTesterRemoveException::elem() {return this->_elem;}
