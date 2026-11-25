cmake_minimum_required(VERSION 3.10)

project(orograph LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)

include(cmake/EnableIPOSupport.cmake)
include(cmake/StaticAnalyzers.cmake)

add_library(orograph src/lib.cpp)
target_include_directories(orograph PUBLIC include)
