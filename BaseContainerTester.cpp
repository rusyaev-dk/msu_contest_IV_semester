#include "BaseContainerTester.h"

using ErrorCode = ContainerTesterException::ErrorCode;

BaseContainerTester::BaseContainerTester(size_t bytes_size) {
    this->_mem_manager = new Mem(bytes_size);
}

void BaseContainerTester::_validate_insertion_code(int err_code) {
    switch (err_code) {
        case 1:
        this->_destroy_container();
        throw ContainerTesterException(ErrorCode::DUPLICATE_INSERT_ERROR);

        case 2:
        this->_destroy_container();
        throw ContainerTesterException(ErrorCode::INSERT_ERROR, "The element was not inserted.");

        default:
        break;
    }
}


BaseContainerTester::~BaseContainerTester() {
    delete this->_mem_manager;
}
