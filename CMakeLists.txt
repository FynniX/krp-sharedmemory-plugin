cmake_minimum_required(VERSION 3.28)
project(
    krp-sharedmemory-plugin
    VERSION 1.0
    LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
include(GNUInstallDirs)
add_library(krp-sharedmemory-plugin SHARED 
    src/KRPSMP.cpp 
    src/Logger/Logger.cpp 
    src/Configuration/Configuration.cpp)
set_target_properties(krp-sharedmemory-plugin PROPERTIES 
    VERSION ${PROJECT_VERSION} 
    SOVERSION ${PROJECT_VERSION_MAJOR}
    PUBLIC_HEADER include/KRPSMP.h)
target_include_directories(krp-sharedmemory-plugin 
    PUBLIC include
    PRIVATE src)
install(TARGETS krp-sharedmemory-plugin
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})