cmake_minimum_required(VERSION 3.17)
project(Barrier)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(Barrier
        Barrier.cpp
        Barrier.h
        barrierdemo.cpp
        Makefile
        README)
