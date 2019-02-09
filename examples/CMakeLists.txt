set(install_dir ${CMAKE_INSTALL_PREFIX}/share/doc/${LIBNAME}/examples)

set(examples
    string_cache
    person_cache
)

foreach(e ${examples})
    add_executable(${e} ${e}.cpp)
    target_link_libraries(${e} ${LIBNAME})
    if ("${cmake_build_type_lower}" STREQUAL "coverage")
        SET_PROPERTY(TARGET ${e} APPEND_STRING PROPERTY LINK_FLAGS " -g --coverage")
    endif()
    install(FILES ${e}.cpp
            DESTINATION ${install_dir})
    add_test(${e}_test ${e})
endforeach()
