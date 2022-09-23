cmake_minimum_required(VERSION 3.17)
project(Condition_variable)

set(CMAKE_CXX_STANDARD 14)

add_executable(Condition_variable
        condition_variable1.cpp
        condition_variable2.cpp
        condition_variable3.cpp)

add_executable(condition_variable1 condition_variable1.cpp)
add_executable(condition_variable2 condition_variable2.cpp)
add_executable(condition_variable3 condition_variable3.cpp)