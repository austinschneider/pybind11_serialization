#include "ABC.h"
#include "pyABC.h"

#include <memory>
#include <iostream>

#include <pybind11/pybind11.h>
#include <pybind11/embed.h>

// Define the module for binding ABC and its trampoline class
PYBIND11_MODULE(pyABC, m) {
    py::class_<ABC, std::shared_ptr<ABC>, pyABC>(m, "ABC")
        // Basic method registration
        .def(py::init<>())
        .def("method", &ABC::method)
        .def("call", &ABC::call)
        // Register a set of pickling methods that store the python state if available
        .def(py::pickle(
            [](const ABC & cpp_obj) {
                py::object self;
                std::cout << "is pyABC instance? " << (dynamic_cast<pyABC const *>(&cpp_obj) != nullptr) << std::endl;
                if(dynamic_cast<pyABC const *>(&cpp_obj) != nullptr and dynamic_cast<pyABC const *>(&cpp_obj)->self) {
                    std::cout << "Grabbing self from pyABC" << std::endl;
                    self = pybind11::reinterpret_borrow<pybind11::object>(dynamic_cast<pyABC const *>(&cpp_obj)->self);
                } else {
                    std::cout << "Grabbing self from global registry" << std::endl;
                    auto *tinfo = pybind11::detail::get_type_info(typeid(ABC));
                    pybind11::handle self_handle = get_object_handle(static_cast<const ABC *>(&cpp_obj), tinfo);
                    self = pybind11::reinterpret_borrow<pybind11::object>(self_handle);
                }
                py::dict d;
                if (py::hasattr(self, "__dict__")) {
                    d = self.attr("__dict__");
                }
                return py::make_tuple(d);
            },
            [](const py::tuple &t) {
                if (t.size() != 1) {
                    throw std::runtime_error("Invalid state!");
                }
                auto cpp_state = std::unique_ptr<ABC>(new pyABC);
                auto py_state = t[0].cast<py::dict>();
                return std::make_pair(std::move(cpp_state), py_state);
            })
        )
        ;
}
