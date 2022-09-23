cmake_minimum_required(VERSION 3.17)
project(Ex3)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS -pthread)

add_executable(Ex3
        Barrier.h Barrier.cpp
        MapReduceClient.h
        MapReduceFramework.h MapReduceFramework.cpp
        SampleClient.cpp)

add_executable(SampleClient MapReduceClient.h SampleClient.cpp)
add_executable(draft draft.cpp)