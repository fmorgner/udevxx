cmake_minimum_required(VERSION 3.14.0)

project(udevxx
    LANGUAGES CXX
    VERSION "1.0.0"
)

set(CMAKE_CXX_STANDARD "17")
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package("PkgConfig")

pkg_check_modules("udev"
    REQUIRED
    IMPORTED_TARGET GLOBAL
    "libudev"
)

# Library

set(LIBRARY_HEADERS
    # public headers
    "${CMAKE_CURRENT_SOURCE_DIR}/include/udevxx/context.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/udevxx/device_enumerator.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/udevxx/device.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/udevxx/tagged_types.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/udevxx/udevxx.hpp"
    # detail headers
    "${CMAKE_CURRENT_SOURCE_DIR}/include/udevxx/detail/raw_type_owner.hpp"
)

add_library("udevxx" INTERFACE)
target_sources("udevxx" INTERFACE
    "$<BUILD_INTERFACE:${LIBRARY_HEADERS}>"
)
target_include_directories("udevxx"
    INTERFACE SYSTEM $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/>
    INTERFACE SYSTEM $<INSTALL_INTERFACE:$<INSTALL_PREFIX>/include/>
)
target_link_libraries("udevxx" INTERFACE
    PkgConfig::udev
)
install(DIRECTORY "include/udevxx"
    DESTINATION "include"
)

# Demos

add_executable("enumerate"
    "src/enumerate.cpp"
)
target_link_libraries("enumerate"
    "udevxx"
)
