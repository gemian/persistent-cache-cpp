if (NOT DEFINED GTEST_ROOT)
    set(GTEST_ROOT /usr/src/gtest)
endif()

set(GTEST_SRC_DIR "${GTEST_ROOT}/src")
set(GTEST_INCLUDE_DIR ${GTEST_ROOT})

add_library(gtest STATIC
    ${GTEST_SRC_DIR}/gtest-all.cc
    ${GTEST_SRC_DIR}/gtest_main.cc
)
set_target_properties(gtest PROPERTIES INCLUDE_DIRECTORIES ${GTEST_INCLUDE_DIR})
target_link_libraries(gtest ${CMAKE_THREAD_LIBS_INIT})

# Make sure we always build gtest with debug info, otherwise we don't
# get line numbers for the code that is dragged in by the test macros.
set_property(TARGET gtest APPEND_STRING PROPERTY COMPILE_FLAGS " -g")

# gtest won't compile cleanly without this.
set_property(TARGET gtest APPEND_STRING PROPERTY COMPILE_FLAGS " -Wno-missing-field-initializers -Wno-old-style-cast")

add_subdirectory(copyright)
add_subdirectory(core)
add_subdirectory(headers)
add_subdirectory(whitespace)

# Tests in subdirectories set this. We push it up to the parent so we can
# exclude the unit tests from coverage.
set(UNIT_TEST_TARGETS ${UNIT_TEST_TARGETS} PARENT_SCOPE)
