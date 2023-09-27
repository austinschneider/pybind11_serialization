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
            [](const py::object &self) {
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
