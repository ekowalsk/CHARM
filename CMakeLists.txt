cmake_minimum_required(VERSION 3.14)
project(charm LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)

include_directories(include)

add_executable(charm
        main.cpp
        src/CharmNode.cpp
        src/Charm.cpp
        src/DCharmNode.cpp
        src/DCharm.cpp
        src/utils.cpp)
