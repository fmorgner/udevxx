find_program(GLIB_RESOURCE_COMPILER
    "glib-compile-resources"
    DOC "Path to glib-compile-resources")

if(NOT GLIB_RESOURCE_COMPILER)
    message(FATAL_ERROR "Failed to find the GLib resource compiler!")
endif()

###############################################################################
# add_glib_resources(<target>
#                    <resource_file>
#                    [RESOURCE_DIRECTORY <dir>])
###############################################################################

function(add_glib_resource TARGET RESOURCE_FILE)
    set(OPTIONS)
    set(SINGLE_VALUE_ARGUMENTS RESOURCE_DIRECTORY)
    set(MULTI_VALUE_ARGUMENTS)

    cmake_parse_arguments(ADD_GLIB_RESOURCE
        "${OPTIONS}"
        "${SINGLE_VALUE_ARGUMENTS}"
        "${MULTI_VALUE_ARGUMENTS}"
        ${ARGN})

    unset(OPTIONS)
    unset(SINGLE_VALUE_ARGUMENTS)
    unset(MULTI_VALUE_ARGUMENTS)

    if(NOT IS_ABSOLUTE "${RESOURCE_FILE}")
        set(RESOURCE_FILE "${CMAKE_CURRENT_SOURCE_DIR}/${RESOURCE_FILE}")
    endif()

    if(NOT EXISTS "${RESOURCE_FILE}")
        message(FATAL_ERROR "Resource file \"${RESOURCE_FILE}\" does not exist!")
    endif()

    if(NOT ADD_GLIB_RESOURCE_RESOURCE_DIRECTORY)
        get_filename_component(ADD_GLIB_RESOURCE_RESOURCE_DIRECTORY "${RESOURCE_FILE}" DIRECTORY)
    endif()


    set(OUTPUT_PATH "${CMAKE_CURRENT_BINARY_DIR}/res/${TARGET}")

    if(NOT EXISTS "${OUTPUT_PATH}")
        file(MAKE_DIRECTORY "${OUTPUT_PATH}")
    endif()

    get_filename_component(RESOURCE_NAME "${RESOURCE_FILE}" NAME_WE)
    get_filename_component(RESOURCE_BASE "${RESOURCE_FILE}" DIRECTORY)

    execute_process(COMMAND ${GLIB_RESOURCE_COMPILER}
        "--generate-dependencies"
        "--sourcedir" "${ADD_GLIB_RESOURCE_RESOURCE_DIRECTORY}"
        "${RESOURCE_FILE}"
        OUTPUT_VARIABLE RESOURCE_DEPENDENCIES
        WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
        OUTPUT_STRIP_TRAILING_WHITESPACE)

    string(REPLACE "\n" ";" RESOURCE_DEPENDENCIES "${RESOURCE_DEPENDENCIES}")

    set(SOURCE_FILE "${OUTPUT_PATH}/${RESOURCE_NAME}.cpp")

    add_custom_command(OUTPUT "${SOURCE_FILE}"
        COMMAND ${GLIB_RESOURCE_COMPILER}
        "--sourcedir" "${ADD_GLIB_RESOURCE_RESOURCE_DIRECTORY}"
        "--generate-source"
        "--target" "${SOURCE_FILE}"
        "${RESOURCE_FILE}"
        DEPENDS "${RESOURCE_FILE}" ${RESOURCE_DEPENDENCIES}
        WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
    )
    target_sources("${TARGET}" PRIVATE "${SOURCE_FILE}")
endfunction()