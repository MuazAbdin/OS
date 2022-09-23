cmake_minimum_required(VERSION 3.17)
project(Ex2)

set(CMAKE_CXX_STANDARD 11)

add_library(Ex2 uthreads.h uthreads.cpp)
add_executable(demo_itimer "Ex2 Resources-20210508/demo_itimer.c")
add_executable(demo_singInt_handler "Ex2 Resources-20210508/demo_singInt_handler.c")
add_executable(demo_jmp "Ex2 Resources-20210508/demo_jmp.c")

add_executable(jona1 jona1.cpp)