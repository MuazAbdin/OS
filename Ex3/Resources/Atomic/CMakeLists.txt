cmake_minimum_required(VERSION 3.17)
project(Atomic)

set(CMAKE_CXX_STANDARD 14)

add_executable(Atomic
        atomicdemo.cpp
        atomicdemo_multivar.cpp
        Makefile
        README)

add_executable(atomicdemo atomicdemo.cpp)
add_executable(atomicdemo_multivar atomicdemo_multivar.cpp)