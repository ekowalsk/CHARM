cmake_minimum_required(VERSION 3.15)

project(CHARM LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)

add_executable(charm
        src/utils.cpp
        src/main.cpp)