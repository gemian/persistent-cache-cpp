add_executable(persistent_string_cache_test persistent_string_cache_test.cpp)
target_link_libraries(persistent_string_cache_test ${TESTLIBS})
add_definitions(-DTEST_DIR="${CMAKE_CURRENT_BINARY_DIR}")

add_test(persistent_string_cache persistent_string_cache_test)
set(TARGETS ${TARGETS} persistent_string_cache_test)

add_executable(speed_test speed_test.cpp)
target_link_libraries(speed_test ${TESTLIBS})
if (${slowtests})
    add_test(speed speed_test)
    set(TARGETS ${TARGETS} speed_test)
endif()

set(UNIT_TEST_TARGETS ${UNIT_TEST_TARGETS} ${TARGETS} PARENT_SCOPE)
