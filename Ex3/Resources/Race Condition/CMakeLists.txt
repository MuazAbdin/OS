cmake_minimum_required(VERSION 3.17)
project(Race_Condition)

set(CMAKE_CXX_STANDARD 14)

add_executable(Race_Condition
        race_condition.cpp
        race_condition_func.cpp
        race_condition_mutex.cpp)

add_executable(race_condition_mutex race_condition_mutex.cpp)
add_executable(race_condition_func race_condition_func.cpp)
add_executable(race_condition race_condition.cpp)