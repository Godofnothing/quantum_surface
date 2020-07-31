# Задать минимальную версию CMake:
cmake_minimum_required(VERSION 2.8)
project(geo_lattice LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)  # Используем C++ 17
set(CMAKE_CXX_FLAGS "-O3 -Wall -pthread")

include_directories(
    include
)

add_executable(
    bin/geo_lattice
    src/main.cpp
)