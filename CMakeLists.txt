cmake_minimum_required(VERSION 3.14.0)

project(udevxx
    LANGUAGES CXX
    VERSION "1.0.0"
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package("PkgConfig")
find_package("Threads")

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
target_compile_features("udevxx" INTERFACE
    "cxx_std_17"
    "cxx_auto_type"
    "cxx_constexpr"
    "cxx_decltype"
    "cxx_defaulted_functions"
    "cxx_generic_lambdas"
    "cxx_lambdas"
    "cxx_noexcept"
    "cxx_nonstatic_member_init"
    "cxx_nullptr"
    "cxx_range_for"
    "cxx_return_type_deduction"
    "cxx_right_angle_brackets"
    "cxx_rvalue_references"
    "cxx_static_assert"
    "cxx_strong_enums"
    "cxx_trailing_return_types"
    "cxx_uniform_initialization"
    "cxx_user_literals"
    "cxx_variable_templates"
    "cxx_variadic_templates"
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
    set_target_properties("enumerate"
        PROPERTIES
        CXX_EXTENSIONS OFF
    )
    target_link_libraries("enumerate" "udevxx")
endif()
