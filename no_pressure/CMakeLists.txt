cmake_minimum_required(VERSION 2.8)

project(FluidNoPressure)

add_definitions(-O3 -std=c++17 -Wall -pedantic -IC:/GLM)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/CMake)



ADD_EXECUTABLE(out src/main)
