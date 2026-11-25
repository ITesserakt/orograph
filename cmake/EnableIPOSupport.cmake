# IPO (interprocedural optimization) works on linking level
# by treating whole project as single compilation unit.
# This enables better inlining.

include(CheckIPOSupported)

check_ipo_supported(RESULT result OUTPUT output)
if(result)
    message("using interprocedural optimisation")
    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
else()
    message(STATUS "IPO not available: ${output}")
endif()
