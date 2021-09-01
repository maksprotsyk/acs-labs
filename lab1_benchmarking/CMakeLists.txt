cmake_minimum_required(VERSION 3.15)
project(productivity_test)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

if (${CMAKE_CXX_COMPILER_ID} MATCHES Clang)
    add_compile_options(-O2)
elseif (${CMAKE_CXX_COMPILER_ID} MATCHES GNU)
    add_compile_options(-O2)
elseif (${CMAKE_CXX_COMPILER_ID} MATCHES MSVC)
    add_compile_options(/O2)
endif()

include_directories(inc)
add_executable(tester src/main.cpp src/fileio.cpp src/methods.cpp)

set(Boost_USE_STATIC_LIBS ON)
find_package(Boost 1.16.0 COMPONENTS REQUIRED)
include_directories(${Boost_INCLUDE_DIR})
