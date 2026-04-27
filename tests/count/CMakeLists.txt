add_executable(alloc_10 alloc_10.c)
target_link_libraries(alloc_10 PRIVATE valgrindcheck)

set(TEST_COUNT_WORKDIR ${CMAKE_BINARY_DIR})

add_test(NAME test_count_10 COMMAND alloc_10 WORKING_DIRECTORY ${TEST_COUNT_WORKDIR})
set_tests_properties(test_count_10 PROPERTIES ENVIRONMENT "VALGRINDCHECK_COUNT=1;VALGRINDCHECK_COUNT_FILENAME=valgrindcheck.count")

add_test(NAME test_count_10_diff COMMAND diff valgrindcheck.count test.out WORKING_DIRECTORY ${TEST_COUNT_WORKDIR})
set_tests_properties(test_count_10_diff PROPERTIES DEPENDS test_count_10)