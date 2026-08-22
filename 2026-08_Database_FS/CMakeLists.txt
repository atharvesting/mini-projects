cmake_minimum_required(VERSION 3.20...4.4)

set(This Spenv)

project(&{This} C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

enable_testing()

add_subdirectory(googletest)

set(Headers
    src/repl.hpp
    )

set(Sources
    src/repl.cpp
    )

add_library(${This} STATIC ${Sources} ${Headers})

add_subdirectory(tests)