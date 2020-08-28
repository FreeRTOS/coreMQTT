# Build Configuration for CMock and Unity libraries.
add_library(cmock STATIC
        "${MODULE_ROOT_DIR}/test/CMock/src/cmock.c"
        )

set_target_properties(cmock PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
        POSITION_INDEPENDENT_CODE ON
        COMPILE_FLAGS "-Og"
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

target_include_directories(cmock PUBLIC
        ${MODULE_ROOT_DIR}/test/CMock/src
        ${MODULE_ROOT_DIR}/test/CMock/vendor/unity/src/
        ${MODULE_ROOT_DIR}/test/CMock/examples
        )

target_link_libraries(cmock unity)
