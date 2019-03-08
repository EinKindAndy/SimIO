cmake_minimum_required(VERSION 3.0 FATAL_ERROR)

project(SimIO)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_executable(testSimIO ./TestSimIO/TestSimIO.cpp)

set_property(TARGET testSimIO PROPERTY CXX_STANDARD 11)