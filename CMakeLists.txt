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
find_package("Threads")

pkg_check_modules("udev"
    REQUIRED
    IMPORTED_TARGET GLOBAL
    "libudev"
)

pkg_check_modules("gtkmm"
    IMPORTED_TARGET GLOBAL
    "gtkmm-3.0"
)

if(gtkmm_FOUND)
    include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules/GlibCompiledResources.cmake")
endif()

# Library

set(LIBRARY_HEADERS
    # public headers
    "${CMAKE_CURRENT_SOURCE_DIR}/include/udevxx/context.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/udevxx/device_enumerator.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/udevxx/device_kind.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/udevxx/device_number.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/udevxx/device.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/udevxx/match_manipulators.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/udevxx/tagged_types.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/udevxx/udevxx.hpp"
    # detail headers
    "${CMAKE_CURRENT_SOURCE_DIR}/include/udevxx/detail/api_utilities.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/udevxx/detail/list.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/udevxx/detail/matches.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/udevxx/detail/raw_type_owner.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/udevxx/detail/thread_aware.hpp"
)

add_library("udevxx" INTERFACE)
target_sources("udevxx" INTERFACE
    "$<BUILD_INTERFACE:${LIBRARY_HEADERS}>"
)
target_include_directories("udevxx" SYSTEM
    INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/>
    INTERFACE $<INSTALL_INTERFACE:$<INSTALL_PREFIX>/include/>
)
target_link_libraries("udevxx" INTERFACE
    PkgConfig::udev
    Threads::Threads
    "stdc++fs"
)
install(DIRECTORY "include/udevxx"
    DESTINATION "include"
)

# Demos

option(BUILD_EXAMPLES "Build example programs" Off)

if(BUILD_EXAMPLES)

    add_executable("enumerate" "src/enumerate/enumerate.cpp")
    target_link_libraries("enumerate" "udevxx")

    if(gtkmm_FOUND)
        add_executable("devtree"
            "${CMAKE_CURRENT_SOURCE_DIR}/src/devtree/device_tree_model.cpp"            
            "${CMAKE_CURRENT_SOURCE_DIR}/src/devtree/devtree.cpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/src/devtree/main_window.cpp"            
        )
        add_glib_resource("devtree"
            "res/devtree/main_window.res"
            HEADER_DIRECTORY
            "${CMAKE_CURRENT_BINARY_DIR}/"
        )
        set(DEVTREE_HEADERS
            "${CMAKE_CURRENT_SOURCE_DIR}/include/devtree/builder_utilities.hpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/include/devtree/device_tree_model.hpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/include/devtree/main_window.hpp"
        )
        add_library("devtree_headers" INTERFACE)
        target_sources("devtree_headers"
            INTERFACE
            "$<BUILD_INTERFACE:${DEVTREE_HEADERS}>"
        )
        target_link_libraries("devtree" "devtree_headers" "udevxx" PkgConfig::gtkmm)
        configure_file("res/devtree/devtree.desktop" "res/devtree/devtree.desktop")
        install(TARGETS "devtree"
            RUNTIME DESTINATION "bin"
        )
        install(PROGRAMS "${CMAKE_CURRENT_BINARY_DIR}/res/devtree/devtree.desktop"
            DESTINATION "${CMAKE_INSTALL_PREFIX}/share/applications"
        )
    endif()

endif()