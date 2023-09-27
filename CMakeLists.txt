cmake_minimum_required(VERSION 3.3.2 FATAL_ERROR)
cmake_policy(VERSION 3.3.2)

if(${CMAKE_HOST_SYSTEM_NAME} MATCHES "Darwin")
    set(MACOSX TRUE)
endif()

# point to appropriate cmake file locations
SET(CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake ${CMAKE_CURRENT_LIST_DIR}/cmake/Packages)

message(STATUS "CMAKE_PREFIX_PATH: ${CMAKE_PREFIX_PATH}")
message(STATUS "CMAKE_INSTALL_PREFIX: ${CMAKE_INSTALL_PREFIX}")

# parse pyproject.toml for the version
include(pyproject)

project(leptoninjector VERSION ${LI_VERSION} LANGUAGES CXX)

# include cmake dependencies
include(GNUInstallDirs)
include(ExternalProject)

SET(CMAKE_CXX_STANDARD 14)
SET(CMAKE_C_STANDARD 99)

# set the build type and appropriate flags
option(CMAKE_BUILD_TYPE "" "Release")
set(_FLAGS "-O2 -Wall -fPIC")
set(_FLAGS_DEBUG "-g -O0 -Wall -fPIC")
set(_FLAGS_RELEASE "-O2 -Wall -fPIC -s")
if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    set(_FLAGS "${_FLAGS} -stdlib=libc++")
    set(_FLAGS_DEBUG "${_FLAGS_DEBUG} -stdlib=libc++")
    set(_FLAGS_RELEASE "${_FLAGS_RELEASE} -stdlib=libc++")
endif()
set(CMAKE_CXX_FLAGS ${_FLAGS})
set(CMAKE_CXX_FLAGS_DEBUG ${_FLAGS_DEBUG})
set(CMAKE_CXX_FLAGS_RELEASE ${_FLAGS_RELEASE})

set(LI_INSTALL_INCLUDEDIR ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_INCLUDEDIR})
set(LI_INSTALL_LIBDIR ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR})

# load python and pybind11
include(Python)
include(pybind11)

# load project dependencies
include(cereal)

add_library(ABC SHARED ABC.cxx)

#pybind11_add_module(pyABC ${PROJECT_SOURCE_DIR}/pyABC.cxx ${PROJECT_SOURCE_DIR}/pyABCbindings.cxx)
pybind11_add_module(pyABC ${PROJECT_SOURCE_DIR}/pyABC.cxx)
target_link_libraries(pyABC PRIVATE ABC pybind11::embed)
pybind11_add_module(pyABCserializer ${PROJECT_SOURCE_DIR}/pyABCbindings.cxx)
target_link_libraries(pyABCserializer PRIVATE ABC pybind11::embed)

add_executable(main main.cxx)
target_link_libraries(main ABC pybind11::embed)

# define the target library
MESSAGE("")
MESSAGE("Done!")
MESSAGE("Run `make` then `make install`")
