cmake_minimum_required(VERSION 4.2)
project(valgrindcheck C)

set(CMAKE_C_STANDARD 17)

set(VALGRINDCHECK_ALLOC_FILENAME "valgrindcheck.alloc")

add_library(valgrindcheck STATIC
        libvalgrindcheck/alloc.c
        libvalgrindcheck/alloc.h)
target_include_directories(valgrindcheck PUBLIC . )
target_compile_definitions(valgrindcheck PUBLIC VALGRINDCHECK_ALLOC_FILENAME=${VALGRINDCHECK_ALLOC_FILENAME})

enable_testing()
add_subdirectory(tests)