cmake_minimum_required(VERSION "3.9.0")

project("udevxx"
    LANGUAGES CXX
    VERSION "1.0.0"
    DESCRIPTION "A C++ wrapper for libudev"
)

include("CMakePackageConfigHelpers")

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
    "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/udevxx/context.hpp>"
    "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/udevxx/device_enumerator.hpp>"
    "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/udevxx/device_kind.hpp>"
    "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/udevxx/device_number.hpp>"
    "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/udevxx/device.hpp>"
    "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/udevxx/match_manipulators.hpp>"
    "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/udevxx/tagged_types.hpp>"
    "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/udevxx/udevxx.hpp>"
    # detail headers
    "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/udevxx/detail/api_utilities.hpp>"
    "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/udevxx/detail/list.hpp>"
    "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/udevxx/detail/matches.hpp>"
    "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/udevxx/detail/raw_type_owner.hpp>"
    "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/udevxx/detail/thread_aware.hpp>"
)

add_library("${PROJECT_NAME}" INTERFACE)
target_sources("${PROJECT_NAME}" INTERFACE
    $<BUILD_INTERFACE:${LIBRARY_HEADERS}>
)
target_include_directories("${PROJECT_NAME}" SYSTEM INTERFACE
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
target_compile_features("${PROJECT_NAME}" INTERFACE
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
target_link_libraries("${PROJECT_NAME}" INTERFACE
    PkgConfig::udev
    Threads::Threads
    "stdc++fs"
)

# Demos

option(BUILD_EXAMPLES "Build example programs" OFF)

if(BUILD_EXAMPLES)
    add_executable("${PROJECT_NAME}_enumerate" "src/enumerate/enumerate.cpp")
    set_target_properties("${PROJECT_NAME}_enumerate"
        PROPERTIES
        CXX_EXTENSIONS OFF
        INTERPROCEDURAL_OPTIMIZATION ON
    )
    target_link_libraries("${PROJECT_NAME}_enumerate" PRIVATE
        "${PROJECT_NAME}"
    )
endif()

# Installation

write_basic_package_version_file(
    "${PROJECT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
    COMPATIBILITY "AnyNewerVersion"
)
configure_package_config_file(
    "${PROJECT_SOURCE_DIR}/cmake/config.cmake.in"
    "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
    INSTALL_DESTINATION "lib/cmake/${PROJECT_NAME}"
)
install(FILES
    "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
    "${PROJECT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
    DESTINATION "lib/cmake/${PROJECT_NAME}"
)
install(TARGETS "${PROJECT_NAME}"
    EXPORT "${PROJECT_NAME}"
    PUBLIC_HEADER DESTINATION "include"
)
if(BUILD_EXAMPLES)
    install(TARGETS "${PROJECT_NAME}_enumerate"
        RUNTIME DESTINATION "bin"
    )
endif()
install(DIRECTORY "include/udevxx"
    DESTINATION "include"
)
install(EXPORT "${PROJECT_NAME}"
    DESTINATION "lib/cmake/${PROJECT_NAME}"
)
