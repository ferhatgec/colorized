# colorized is just header only simple library, only thing you need is include.
# this cmake module will compile example.cpp
cmake_minimum_required(VERSION 3.22)
project(colorized_example)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(colorized_example example.cpp)
target_include_directories(colorized_example PRIVATE _legacy/Colorized.hpp)