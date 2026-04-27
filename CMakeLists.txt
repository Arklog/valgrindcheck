cmake_minimum_required(VERSION 4.2)
project(valgrindcheck C CXX)

set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 23)

include(FetchContent)

fetchcontent_declare(
        indicators
        GIT_REPOSITORY https://github.com/p-ranav/indicators
        GIT_TAG v2.3
)

fetchcontent_makeavailable(
        indicators
)

add_library(valgrindcheck STATIC
        libvalgrindcheck/alloc.c
        libvalgrindcheck/alloc.h)
target_include_directories(valgrindcheck PUBLIC . )

add_executable(valgrindcheck_bin
        valgrindcheck/main.cpp
        valgrindcheck/Env.cpp
        valgrindcheck/Env.hpp
        valgrindcheck/Process.cpp
        valgrindcheck/Process.hpp
        valgrindcheck/ProcessStatus.cpp
        valgrindcheck/ProcessStatus.hpp
        valgrindcheck/ProcessPool.cpp
        valgrindcheck/ProcessPool.hpp)
target_include_directories(valgrindcheck_bin PUBLIC valgrindcheck)
target_link_libraries(valgrindcheck_bin PRIVATE indicators)

enable_testing()
add_subdirectory(tests)