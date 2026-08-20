cmake_minimum_required(VERSION 3.20)

project(database_fs)

add_executable(
    database_fs src/entry.cpp
    src/repl.cpp
    src/repl.hpp)