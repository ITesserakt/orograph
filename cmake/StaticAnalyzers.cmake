# Include clang-tidy if available
find_program(CLANGTIDY clang-tidy)
if(CLANGTIDY)
    message("using clang-tidy (${CLANGTIDY})")
    set(CMAKE_CXX_CLANG_TIDY ${CLANGTIDY})
else()
    message(STATUS "clang-tidy not available")
endif()

# Include cppcheck if available
find_program(CPPCHECK cppcheck)
if(CPPCHECK)
    message("using cppcheck (${CPPCHECK})")
    set(CMAKE_CXX_CPPCHECK ${CPPCHECK})
else()
    message(STATUS "cppcheck not available")
endif()