cmake_minimum_required(VERSION 3.15)
project(integral_calculator)

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
add_executable(calculator src/main.cpp inc/integrals.h src/integrals.cpp inc/time_measurements.h inc/tests.h src/tests.cpp)

find_package(Threads REQUIRED)
target_link_libraries(calculator Threads::Threads)

