cmake_minimum_required(VERSION 3.5)
project(valgrindcheck C CXX)

set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 23)

include(FetchContent)

fetchcontent_declare(
        indicators
        GIT_REPOSITORY https://github.com/p-ranav/indicators
        GIT_TAG v2.3
)

FetchContent_Declare(
        argparse
        GIT_REPOSITORY https://github.com/Taywee/args
        GIT_TAG 6.4.12
)

FetchContent_Declare(
        spdlog
        GIT_REPOSITORY https://github.com/gabime/spdlog
        GIT_TAG v1.15.3
)

fetchcontent_makeavailable(
        indicators
        argparse
        spdlog
)

add_library(valgrindcheck STATIC
        libvalgrindcheck/alloc.c)
target_include_directories(valgrindcheck PUBLIC .)

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
target_link_libraries(valgrindcheck_bin PRIVATE indicators taywee::args spdlog::spdlog)

enable_testing()
add_subdirectory(tests)
