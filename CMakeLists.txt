cmake_minimum_required(VERSION 4.4)

project(orograph VERSION 0.0.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
enable_testing()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20 -Wall -Wextra -Werror")

#include(cmake/EnableIPOSupport.cmake)
include(cmake/StaticAnalyzers.cmake)
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)
include(GenerateExportHeader)

find_package(Eigen3 5.0...6.0 REQUIRED)

add_subdirectory(lib/orograph-internal)

add_library(orograph src/capi.cpp)
generate_export_header(orograph
        EXPORT_FILE_NAME "${CMAKE_CURRENT_BINARY_DIR}/orograph_export.h"
)
target_sources(orograph
        PUBLIC
        FILE_SET HEADERS
        BASE_DIRS "include" "${CMAKE_CURRENT_BINARY_DIR}"
        FILES
        "include/orograph.h"
        "${CMAKE_CURRENT_BINARY_DIR}/orograph_export.h"
)
target_link_libraries(orograph PRIVATE $<BUILD_INTERFACE:orograph-internal>)
set_target_properties(orograph PROPERTIES
        CXX_VISIBILITY_PRESET hidden
        VISIBILITY_INLINES_HIDDEN ON
)
if (NOT BUILD_SHARED_LIBS)
    target_compile_definitions(orograph PUBLIC OROGRAPH_STATIC_DEFINE)
endif ()

add_executable(orograph-test
        test/example.cpp
        test/runner.cpp
)
target_link_libraries(orograph-test PRIVATE orograph-internal)
discover_tests(
        COMMAND orograph-test
        DISCOVERY_ARGS --list
        DISCOVERY_MATCH "^ - ([A-Za-z0-9_]+)( \\[.*\\])?$"
        TEST_NAME "${PROJECT_NAME}.\\1"
        TEST_ARGS "\\1" "--exact"
)

install(TARGETS orograph
        EXPORT orographTargets
        FILE_SET HEADERS
)

install(EXPORT orographTargets
        FILE orographTargets.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/orograph
)

configure_package_config_file(
        cmake/orographConfig.cmake.in
        ${CMAKE_CURRENT_BINARY_DIR}/orographConfig.cmake
        INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/orograph
)
write_basic_package_version_file(
        ${CMAKE_CURRENT_BINARY_DIR}/orographConfigVersion.cmake
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY SameMajorVersion
)

install(FILES
        ${CMAKE_CURRENT_BINARY_DIR}/orographConfig.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/orographConfigVersion.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/orograph
)