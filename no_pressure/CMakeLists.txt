cmake_minimum_required(VERSION 2.8)

project(FluidNoPressure)

add_definitions(-std=c++17 -IC:/GLM)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/CMake)



ADD_EXECUTABLE(out src/main)
