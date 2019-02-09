add_executable(persistent_cache_test persistent_cache_test.cpp)
target_link_libraries(persistent_cache_test ${TESTLIBS})
add_definitions(-DTEST_DIR="${CMAKE_CURRENT_BINARY_DIR}")

add_test(persistent_cache persistent_cache_test)
set(TARGETS ${TARGETS} persistent_cache_test)

set(UNIT_TEST_TARGETS ${UNIT_TEST_TARGETS} ${TARGETS} PARENT_SCOPE)
