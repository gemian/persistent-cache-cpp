include(FindPkgConfig)
find_package(Boost COMPONENTS system filesystem)

set(TESTLIBS ${LIBNAME} boost_filesystem boost_system leveldb gtest)

add_subdirectory(persistent_cache)
add_subdirectory(persistent_string_cache)
add_subdirectory(internal)

set(UNIT_TEST_TARGETS ${UNIT_TEST_TARGETS} PARENT_SCOPE)
