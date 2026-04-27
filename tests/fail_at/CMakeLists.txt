add_executable(fail_at_0 fail_at.c)
target_compile_definitions(fail_at_0 PRIVATE FAIL_AT=0)
target_link_libraries(fail_at_0 PRIVATE valgrindcheck)

add_test(NAME fail_at_0_test COMMAND fail_at_0)
set_tests_properties(fail_at_0_test PROPERTIES ENVIRONMENT "VALGRINDCHECK_FAIL_AT=0")

add_executable(fail_at_1 fail_at.c)
target_compile_definitions(fail_at_1 PRIVATE FAIL_AT=1)
target_link_libraries(fail_at_1 PRIVATE valgrindcheck)

add_test(NAME fail_at_1_test COMMAND fail_at_1)
set_tests_properties(fail_at_1_test PROPERTIES ENVIRONMENT "VALGRINDCHECK_FAIL_AT=1")


add_executable(fail_at_9 fail_at.c)
target_compile_definitions(fail_at_9 PRIVATE FAIL_AT=9)
target_link_libraries(fail_at_9 PRIVATE valgrindcheck)

add_test(NAME fail_at_9_test COMMAND fail_at_9)
set_tests_properties(fail_at_9_test PROPERTIES ENVIRONMENT "VALGRINDCHECK_FAIL_AT=9")