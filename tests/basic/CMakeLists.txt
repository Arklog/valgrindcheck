add_executable(basic_bin main.c)
target_link_libraries(basic_bin PRIVATE valgrindcheck)

add_custom_target(basic_clear_valgrindcheck rm ${CMAKE_CURRENT_SOURCE_DIR}/valgrindcheck.alloc)

add_test(basic_bin_0 basic_bin 0 WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
set_tests_properties(basic_bin_0 PROPERTIES DEPENDS basic_clear_valgrindcheck)
add_test(basic_bin_1 basic_bin 1 WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
add_test(basic_bin_2 basic_bin 2 WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
add_test(basic_bin_3 basic_bin 3 WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
add_test(basic_bin_4 basic_bin 4 WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})