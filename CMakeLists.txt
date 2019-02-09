cmake_minimum_required(VERSION 2.8.11)

# Default install location. Must be set here, before setting the project.
if (NOT DEFINED CMAKE_INSTALL_PREFIX)
    set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/install CACHE PATH "" FORCE)
endif()

project(persistent-cache-cpp C CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(LIBNAME persistent-cache-cpp)

set(VERSION_MAJOR 1)
set(VERSION_MINOR 0)
set(VERSION_MICRO 4)
set(LIBVERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_MICRO}")

string(TOLOWER "${CMAKE_BUILD_TYPE}" cmake_build_type_lower) # Build types should always be lower case

set(ACCEPTED_BUILD_TYPES "" none release debug relwithdebinfo coverage)
list(FIND ACCEPTED_BUILD_TYPES "${cmake_build_type_lower}" IS_BUILD_TYPE_ACCEPTED)
if(${IS_BUILD_TYPE_ACCEPTED} EQUAL -1)
    message(FATAL_ERROR "Invalid CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}\nValid types are: ${ACCEPTED_BUILD_TYPES}")
endif()

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --std=c++11 -Wall -pedantic -Wextra")

# Some additional warnings not included by the general flags set above.
set(EXTRA_C_WARNINGS "-Wcast-align -Wcast-qual -Wformat -Wredundant-decls -Wswitch-default")
set(EXTRA_CXX_WARNINGS "-Wnon-virtual-dtor -Wctor-dtor-privacy -Wold-style-cast")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${EXTRA_C_WARNINGS} ${EXTRA_CXX_WARNINGS}")

# By default, for release builds, warnings become hard errors.
if ("${cmake_build_type_lower}" STREQUAL "release" OR "${cmake_build_type_lower}" STREQUAL "relwithdebinfo")
    option(Werror "Treat warnings as errors" ON)
else()
    option(Werror "Treat warnings as errors" OFF)
endif()

# If warnings are errors, don't error on deprecated declarations.
if (${Werror})
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror")
    if ("${cmake_build_type_lower}" STREQUAL "release" OR "${cmake_build_type_lower}" STREQUAL "relwithdebinfo")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-error=deprecated-declarations")
    endif()
endif()

# Flags for thread/address sanitizer
set(SANITIZER "" CACHE STRING "Build with -fsanitize=<value> (legal values: thread, address)")

if ("${SANITIZER}" STREQUAL "")
    # Do nothing
elseif (${SANITIZER} STREQUAL "thread")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=thread -fno-omit-frame-pointer -g")
elseif (${SANITIZER} STREQUAL "address")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -fno-omit-frame-pointer -g")
else()
    message(FATAL_ERROR "Invalid SANITIZER setting: ${SANITIZER}")
endif()

# Some tests are slow, so make it possible not to run them
# during day-to-day development.
option(slowtests "Run slow tests" ON)

# Definitions for testing with valgrind.

configure_file(CTestCustom.cmake.in CTestCustom.cmake) # Tests in CTestCustom.cmake are skipped for valgrind

find_program(MEMORYCHECK_COMMAND NAMES valgrind)
if (MEMORYCHECK_COMMAND)
    set(MEMORYCHECK_COMMAND_OPTIONS
        "--suppressions=${CMAKE_SOURCE_DIR}/valgrind-suppress --errors-for-leak-kinds=definite --show-leak-kinds=definite --leak-check=full --num-callers=50 --error-exitcode=3"
    )
    add_custom_target(valgrind DEPENDS NightlyMemCheck)
else()
    message(WARNING "Cannot find valgrind: valgrind target will not be available")
endif()

include(CTest)
enable_testing()

find_package(CoverageReport)
find_package(Boost COMPONENTS filesystem REQUIRED)
find_package(Threads REQUIRED)

include_directories(include)

add_subdirectory(src)
add_subdirectory(include)
add_subdirectory(tests)
add_subdirectory(data)
add_subdirectory(examples)
add_subdirectory(doc)

enable_coverage_report(
    TARGETS
        ${LIBNAME}
        ${UNIT_TEST_TARGETS}
    FILTER
        ${CMAKE_SOURCE_DIR}/tests/*
        ${CMAKE_BINARY_DIR}/*
    TESTS
        ${UNIT_TEST_TARGETS}
)

find_package(DoxygenBuilder)

file(GLOB public_headers ${CMAKE_SOURCE_DIR}/include/core/*.h)
add_doxygen(
    ${LIBNAME}-doc
    INPUT
        ${CMAKE_BINARY_DIR}/doc/main_page.dox
        ${public_headers}
    OUTPUT_DIRECTORY
        ${CMAKE_BINARY_DIR}/doc
    STRIP_FROM_PATH
        "${CMAKE_SOURCE_DIR}/src"
    STRIP_FROM_INC_PATH
        "${CMAKE_SOURCE_DIR}/include"
    EXCLUDE_PATTERNS
        */internal/*
    EXCLUDE_SYMBOLS
        *::internal*
        *::Priv
    ALL
)
install(DIRECTORY ${CMAKE_BINARY_DIR}/doc/html
        DESTINATION share/doc/${LIBNAME})
