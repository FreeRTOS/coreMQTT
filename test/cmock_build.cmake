# Macro utility to clone the CMock submodule.
macro( clone_cmock )
        find_package( Git REQUIRED )
        message( "Cloning submodule CMock." )
        execute_process( COMMAND rm -rf ${MODULE_ROOT_DIR}/test/CMock
                         COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive ${MODULE_ROOT_DIR}/test/CMock
                        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
                        RESULT_VARIABLE CMOCK_CLONE_RESULT )

        if( NOT ${CMOCK_CLONE_RESULT} STREQUAL "0" )
                message( FATAL_ERROR "Failed to clone CMock submodule." )
        endif()
endmacro()

# Macro utility to add library targets for Unity and CMock to build configuration.
macro( add_cmock_targets )
        # Build Configuration for CMock and Unity libraries.
        list( APPEND CMOCK_INCLUDE_DIRS
                "${MODULE_ROOT_DIR}/test/CMock/vendor/unity/src/"
                "${MODULE_ROOT_DIR}/test/CMock/vendor/unity/extras/fixture/src"
                "${MODULE_ROOT_DIR}/test/CMock/vendor/unity/extras/memory/src"
                "${MODULE_ROOT_DIR}/test/CMock/src"
        )

        add_library(cmock STATIC
        "${MODULE_ROOT_DIR}/test/CMock/src/cmock.c"
        )

        set_target_properties(cmock PROPERTIES
                ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
                POSITION_INDEPENDENT_CODE ON
                COMPILE_FLAGS "-Og"
                )

        target_include_directories(cmock PUBLIC
                ${MODULE_ROOT_DIR}/test/CMock/src
                ${MODULE_ROOT_DIR}/test/CMock/vendor/unity/src/
                ${MODULE_ROOT_DIR}/test/CMock/examples
                ${CMOCK_INCLUDE_DIRS}
                )

        add_library(unity STATIC
                "${MODULE_ROOT_DIR}/test/CMock/vendor/unity/src/unity.c"
                "${MODULE_ROOT_DIR}/test/CMock/vendor/unity/extras/fixture/src/unity_fixture.c"
                "${MODULE_ROOT_DIR}/test/CMock/vendor/unity/extras/memory/src/unity_memory.c"
                )

        set_target_properties(unity PROPERTIES
                ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
                POSITION_INDEPENDENT_CODE ON
                )

        target_include_directories(unity PUBLIC
                ${CMOCK_INCLUDE_DIRS}
                )

        target_link_libraries(cmock unity)
endmacro()
