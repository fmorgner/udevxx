cmake_minimum_required(VERSION 3.14.0)

project(udevxx
    LANGUAGES CXX
    VERSION "1.0.0"
)

set(CMAKE_CXX_STANDARD "17")
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package("PkgConfig")

pkg_check_modules("udev"
    REQUIRED
    IMPORTED_TARGET GLOBAL
    "libudev"
)

add_executable("udevxx_demo"
    "src/demo.cpp"
)
target_link_libraries("udevxx_demo"
    PkgConfig::udev
)
target_include_directories("udev_demo"
    "include"
)