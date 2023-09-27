#include "ABC.h"

#include <iostream>

#include <pybind11/pybind11.h>
#include <pybind11/embed.h>

namespace py = pybind11;

#define C_PYBIND11_OVERRIDE(selfname, BaseType, returnType, cfuncname, pyfuncname) \
        BaseType * ref; \
        if(selfname) { \
            ref = selfname.cast<BaseType *>(); \
        } else { \
            ref = this; \
        } \
        do { \
            do { \
                auto *tinfo = pybind11::detail::get_type_info(typeid(BaseType)); \
                pybind11::function override = \
                    tinfo ? pybind11::detail::get_type_override(static_cast<const BaseType *>(ref), tinfo, pyfuncname) : pybind11::function(); \
                if (override) { \
                    auto o = override(); \
                    if (pybind11::detail::cast_is_temporary_value_reference<returnType>::value) { \
                        static pybind11::detail::override_caster_t<returnType> caster; \
                        return pybind11::detail::cast_ref<returnType>(std::move(o), caster); \
                    } \
                    return pybind11::detail::cast_safe<returnType>(std::move(o)); \
                } \
            } while (false); \
            pybind11::pybind11_fail( \
                "Tried to call pure virtual function \"" PYBIND11_STRINGIFY(BaseType) "::" "cfuncname" "\""); \
        } while (false);

#define C_PYBIND11_SAVELOAD(selfname, BaseType) \
    void load_py(std::string str) { \
        py::bytes bytes = (py::bytes)(str); \
        py::module pkl = py::module::import("pickle"); \
        py::module mod = py::module::import("module"); \
        this->selfname = pkl.attr("loads")(bytes); \
    } \
    std::string save_py() { \
        pybind11::object obj; \
        if(this->selfname) { \
            obj = this->selfname; \
        } else { \
            auto *tinfo = pybind11::detail::get_type_info(typeid(BaseType)); \
            pybind11::handle self_handle = get_object_handle(static_cast<const BaseType *>(this), tinfo); \
            pybind11::object obj = pybind11::object(self_handle, true); \
        } \
        std::cout << "importing pickle" << std::endl; \
        py::module pkl = py::module::import("pickle"); \
        std::cout << "imported pickle" << std::endl; \
        std::cout << "importing module" << std::endl; \
        py::module mod = py::module::import("module"); \
        std::cout << "imported module" << std::endl; \
        std::cout << "dumping obj" << std::endl; \
        py::bytes bytes = pkl.attr("dumps")(obj); \
        std::cout << "dumped obj" << std::endl; \
        std::string str = (std::string)(bytes); \
        return str; \
    }

class pyABC : public ABC {
public:
    using ABC::ABC;
    pyABC(ABC && parent) : ABC(std::move(parent)) {}
    pybind11::object self;
    // C_PYBIND11_SAVELOAD(self, ABC)

    void method() override {
        C_PYBIND11_OVERRIDE(
                self,
                ABC,
                void,
                method,
                "method"
                )
    }

    void call() override {
        PYBIND11_OVERLOAD(
            void,
            ABC,
            call,
        );
    }
};