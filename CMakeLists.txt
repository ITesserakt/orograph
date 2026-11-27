cmake_minimum_required(VERSION 3.10)

project(orograph LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
enable_testing()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror")

include(cmake/EnableIPOSupport.cmake)
include(cmake/StaticAnalyzers.cmake)

add_library(orograph src/lib.cpp)
target_include_directories(orograph PUBLIC include)

add_executable(orograph-test
        test/example.cpp
        test/runner.cpp)
target_link_libraries(orograph-test PRIVATE orograph)
add_test(NAME orograph-all_tests COMMAND orograph-test --all)
