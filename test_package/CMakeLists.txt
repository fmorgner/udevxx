cmake_minimum_required(VERSION "3.9.0")

project("PackageTest"
    LANGUAGES CXX
    DESCRIPTION "Conan udevxx test package"
)

find_package("Threads")

include("${CMAKE_BINARY_DIR}/conanbuildinfo.cmake")

conan_basic_setup(TARGETS)

add_executable("${PROJECT_NAME}"
    "src/package_test.cpp"
)

target_link_libraries("${PROJECT_NAME}"
    "CONAN_PKG::udevxx"
    "Threads::Threads"
)

# set_target_properties("${PROJECT_NAME}"
#     PROPERTIES
#     CXX_EXTENSIONS OFF
# )