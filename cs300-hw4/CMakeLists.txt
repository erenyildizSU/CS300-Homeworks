cmake_minimum_required(VERSION 3.27)
project(cs300_hw4)

set(CMAKE_CXX_STANDARD 17)

add_executable(cs300_hw4 main.cpp
        2.cpp
        cmake-build-debug/yedek.cpp
        cmake-build-debug/yedek.h
        3.cpp)
