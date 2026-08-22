cmake_minimum_required(VERSION 3.20...4.4)

set(This SpenvTests)

set(Sources
    test.cpp
)

add_executable(${This} ${Sources})

target_link_libraries(${This} PUBLIC
    gtest_main
    Spenv
    )

add_test(
    NAME ${This}
    COMMAND ${This}
)
