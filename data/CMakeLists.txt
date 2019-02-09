# Set up package config.
set(PKGCONFIG_NAME ${LIBNAME})
set(PKGCONFIG_DESCRIPTION "A cache of key-value pairs with persistent storage for C++")
set(VERSION "1.0")

configure_file(lib${LIBNAME}.pc.in lib${LIBNAME}.pc @ONLY)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/lib${LIBNAME}.pc DESTINATION lib/${CMAKE_LIBRARY_ARCHITECTURE}/pkgconfig)
