#include "ABC.h"

#include <iostream>

void ABC::method() {
    std::cout << "[C++   ] method called [" << this << "]" << std::endl;
}

void ABC::call() {
    this->method();
}

ABC::~ABC() {
    std::cout << "[C++   ] destructor called [" << this << "]" << std::endl;
}
