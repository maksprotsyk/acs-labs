cmake_minimum_required(VERSION 3.15)
project(MPI_lab)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)


# Options
option(WARNINGS_AS_ERRORS   "Treat compiler warnings as errors."    ON)
option(ENABLE_CONAN         "Use Conan as a package manager."       OFF)
option(ENABLE_PVS_STUDIO    "Use PVS-Studio static code analyzer."  OFF) # Option for the local usage only. PVS-Studio isn't installed on GitHub machines.
option(ENABLE_SANITIZERS    "Use sanitizers to detect errors."      OFF) # Option for the test builds. Do not use it in the production builds.

include_directories(inc)

add_executable(MPI_lab src/main.cpp src/mpi_modules.cpp src/temperatures.cpp src/TwoDimArray.cpp)

set (Boost_USE_STATIC_LIBS OFF)
set (Boost_USE_MULTITHREAD ON)

find_package(Boost REQUIRED mpi serialization program_options)
target_link_libraries(MPI_lab ${Boost_LIBRARIES})
include_directories(${Boost_INCLUDE_DIRS})

find_package(MPI REQUIRED)
include_directories(${MPI_CXX_INCLUDE_PATH})
target_link_libraries(MPI_lab ${MPI_CXX_LIBRARIES})
