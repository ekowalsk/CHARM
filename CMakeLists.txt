cmake_minimum_required(VERSION 3.14)
project(projekt)

set(CMAKE_CXX_STANDARD 14)

add_executable(projekt main.cpp include/CharmNode.h src/CharmNode.cpp include/Charm.h src/Charm.cpp include/DCharmNode.h src/DCharmNode.cpp include/DCharm.h src/DCharm.cpp)

include_directories(./include)