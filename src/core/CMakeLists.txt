find_library(LEVELDB NAMES leveldb REQUIRED)

add_subdirectory(internal)

set(CACHE_SRC
    ${CACHE_SRC}
    ${CMAKE_CURRENT_SOURCE_DIR}/persistent_cache_stats.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/persistent_string_cache.cpp
)

add_library(${LIBNAME} STATIC ${CACHE_SRC})
target_link_libraries(${LIBNAME} ${LEVELDB})

install(TARGETS ${LIBNAME}
        DESTINATION lib/${CMAKE_LIBRARY_ARCHITECTURE})
