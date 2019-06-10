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

add_library("udevxx"
    "src/udevxx/context.cpp"
    "src/udevxx/detail/device_iterator.cpp"
    "src/udevxx/device.cpp"
    "src/udevxx/enumerator.cpp"
    "src/udevxx/subsystem.cpp"
    "src/udevxx/syspath.cpp"
)
target_link_libraries("udevxx"
    PkgConfig::udev
)
target_include_directories("udevxx"
    SYSTEM PUBLIC
    "include"
)

install(TARGETS "udevxx")
install(DIRECTORY "include/udevxx"
    DESTINATION "include"
    FILES_MATCHING PATTERN "*.hpp"
)

# Demos

add_executable("udevxx_demo"
    "src/demo.cpp"
)
target_link_libraries("udevxx_demo"
    "udevxx"
)
