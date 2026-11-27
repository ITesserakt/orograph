add_library(orograph-internal OBJECT
        src/lib.cpp
)
target_include_directories(orograph-internal
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)
target_link_libraries(orograph-internal PUBLIC Eigen3::Eigen)
set_target_properties(orograph-internal PROPERTIES
        CXX_VISIBILITY_PRESET hidden
        VISIBILITY_INLINES_HIDDEN ON
        POSITION_INDEPENDENT_CODE ON
)
