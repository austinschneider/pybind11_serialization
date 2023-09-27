#include "ABC.h"

#include <iostream>

void ABC::method() {
    std::cout << "[C++   ] method called [" << this << "]" << std::endl;
}

void ABC::call() {
    this->method();
}

pybind11::object ABC::get_self() {
    return pybind11::cast<pybind11::none>(Py_None);
}

ABC::~ABC() {
    std::cout << "[C++   ] destructor called [" << this << "]" << std::endl;
}
