cmake_minimum_required(VERSION 3.15)
project(dict_builder)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)


#option(WARNINGS_AS_ERRORS   "Treat compiler warnings as errors."    ON)
#option(ENABLE_CONAN         "Use Conan as a package manager."       OFF)
#option(ENABLE_PVS_STUDIO    "Use PVS-Studio static code analyzer."  OFF) # Option for the local usage only. PVS-Studio isn't installed on GitHub machines.
#option(ENABLE_SANITIZERS    "Use sanitizers to detect errors."      OFF) # Option for the test builds. Do not use it in the production builds.

include_directories(inc)
add_executable(
    prog src/main.cpp src/config.cpp src/files.cpp src/processing.cpp
    inc/safe_queue.h inc/config.h inc/files.h inc/processing.h inc/time_measurements.h)


find_package(LibArchive REQUIRED)
find_package(ICU REQUIRED COMPONENTS uc i18n)
find_package(TBB REQUIRED COMPONENTS tbb)


set (Boost_USE_STATIC_LIBS OFF)
set (Boost_USE_MULTITHREAD ON)
find_package(
    Boost REQUIRED COMPONENTS
    filesystem locale system
)

include_directories(${Boost_INCLUDE_DIR} ${LibArchive_INCLUDE_DIRS} ${TBB_INCLUDE_DIR})

target_link_libraries(prog ${Boost_LIBRARIES} ${ICU_LIBRARIES} ${LibArchive_LIBRARIES} TBB::tbb)

