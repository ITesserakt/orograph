cmake_minimum_required(VERSION 3.23)

project(orograph VERSION 0.0.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
enable_testing()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20 -Wall -Wextra -Werror")

include(cmake/EnableIPOSupport.cmake)
include(cmake/StaticAnalyzers.cmake)
include(GNUInstallDirs)

find_package(Eigen3 5.0...6.0 REQUIRED)

add_library(orograph STATIC
        src/lib.cpp
)
target_sources(orograph
        PUBLIC
        FILE_SET HEADERS
        BASE_DIRS "include"
        FILES
        "include/lib.h"
        "include/platform.h"
)
target_link_libraries(orograph PRIVATE Eigen3::Eigen)

add_executable(orograph-test
        test/example.cpp
        test/runner.cpp
)
target_link_libraries(orograph-test PRIVATE orograph)
add_test(NAME orograph-all_tests COMMAND orograph-test --all)

install(TARGETS orograph
        EXPORT orographTargets
        FILE_SET HEADERS
)

install(EXPORT orographTargets
        FILE orographTargets.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/orograph
)