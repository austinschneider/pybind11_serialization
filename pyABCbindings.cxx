#include "ABC.h"
#include "pyABC.h"

#include <iostream>

#include <pybind11/pybind11.h>
#include <pybind11/embed.h>

namespace py = pybind11;

std::string pyABC_dumper(pyABC & object) {
    pybind11::object obj;
    if(object.self) {
        obj = object.self;
    } else {
        auto *tinfo = pybind11::detail::get_type_info(typeid(ABC));
        pybind11::handle self_handle = get_object_handle(static_cast<const ABC *>(&object), tinfo);
        obj = pybind11::reinterpret_borrow<pybind11::object>(self_handle);
    }
    std::cout <<"#########" << std::endl;
    std::cout << "importing pickle" << std::endl;
    py::module pkl = py::module::import("pickle");
    if(pkl)
        std::cout << "imported pickle" << std::endl;
    else
        std::cout << "failed to import pickle" << std::endl;
    std::cout << "dumping obj" << std::endl;
    py::bytes bytes = pkl.attr("dumps")(obj);
    std::cout << "dumped obj" << std::endl;
    std::string str = (std::string)(bytes.attr("hex")().cast<std::string>());
    return str;
}

pyABC pyABC_loader(std::string & state) {
    pyABC object;
    std::cout << "importing pickle" << std::endl;
    py::module pkl = py::module::import("pickle");

    std::cout << "getting fromhex" << std::endl;
    py::object fromhex = py::globals()["__builtins__"].attr("bytes").attr("fromhex");
    std::cout << "calling fromhex" << std::endl;
    py::object bytes = fromhex(state);
    py::print(bytes);

    std::cout << "loading from bytes" << std::endl;
    pkl.attr("loads")(bytes);
    std::cout << "storing from bytes" << std::endl;
    object.self = pkl.attr("loads")(bytes);
    std::cout << "returning" << std::endl;
    return object;
}

PYBIND11_MODULE(pyABCserializer, m) {
    m.def("save", &pyABC_dumper);
    m.def("load", &pyABC_loader);
}
