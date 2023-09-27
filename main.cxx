#include <iostream>
#include <pybind11/pybind11.h>
#include <pybind11/embed.h>
#include "ABC.h"

namespace py = pybind11;

void log(const std::string& msg) {
    std::cout << "[C++   ] " << msg + '\n';
}

/**
* Implements CppClass interface by calling stored py::object.
* Stored py::object extends python object's lifetime.
*/
class ABCWrapper : public ABC {
public:
  ABCWrapper(py::object pyClass): m_pyClass(std::move(pyClass)){}
  virtual ~ABCWrapper() = default;
  virtual void method(){
      m_pyClass.attr("method")();
  }
private:
  py::object m_pyClass;
};

ABC* user_defined_function(bool incref) {
    py::module module = py::module::import("module");
    py::object obj = module.attr("PyDerived")();
    if (incref) { obj.inc_ref(); }
    log("end of user_defined_function");
    return new ABCWrapper(obj); // implicit cast to CppClass*
}

int main() {
    py::scoped_interpreter guard{};
    {
        log("TEST 1: no factory function");
        py::module module = py::module::import("module");
        py::object obj = module.attr("PyDerived")();
        log("calling method on python object");
        obj.attr("method")();
        log("calling method on cpp pointer");
        ABC *instance1 = obj.cast<ABC *>();
        instance1->method();
        log("end of scoped code");
    }
    std::cout << std::endl;
    log("TEST 2: no incref");
    ABC* instance2 = user_defined_function(false);
    // commented out to avoid segmentation fault
    log("calling method on cpp pointer");
    instance2->method();
    delete instance2;

    std::cout << std::endl;
    log("TEST 3 : incref");
    ABC* instance3 = user_defined_function(true);
    log("calling method on cpp pointer");
    instance3->method();
    log("deleting instance3");
    delete instance3;

    std::cout << std::endl;
    log("TEST 4 : incref and decref");
    ABC* instance4 = user_defined_function(true);
    log("calling method on cpp pointer");
    instance4->method();
    log("decrementing ref count on instance4 wrapped python object");
    py::object obj4 = py::cast(instance4);
    obj4.dec_ref();

    std::cout << std::endl;
    log("TEST 5 : incref and decref twice");
    ABC* instance5 = user_defined_function(true);
    log("calling method on cpp pointer");
    instance5->method();
    log("decrementing ref count on instance5 wrapped python object (twice)");
    py::object obj5 = py::cast(instance5);
    obj5.dec_ref();
    obj5.dec_ref();

    log("End of main");
    return 0;
}
