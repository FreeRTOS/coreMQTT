# Project information.
cmake_minimum_required( VERSION 3.13.0 )
project( coreMqtt
         VERSION 4.0.2
         LANGUAGES C )

# Allow the project to be organized into folders.
set_property( GLOBAL PROPERTY USE_FOLDERS ON )

# Use C90.
set( CMAKE_C_STANDARD 90 )
set( CMAKE_C_STANDARD_REQUIRED ON )

# Do not allow in-source build.
if( ${PROJECT_SOURCE_DIR} STREQUAL ${PROJECT_BINARY_DIR} )
    message( FATAL_ERROR "In-source build is not allowed. Please build in a separate directory, such as ${PROJECT_SOURCE_DIR}/build." )
endif()

# Set global path variables.
get_filename_component(__MODULE_ROOT_DIR "${CMAKE_CURRENT_LIST_DIR}" ABSOLUTE)
set(MODULE_ROOT_DIR ${__MODULE_ROOT_DIR} CACHE INTERNAL "MQTT source root.")

# Configure options to always show in CMake GUI.
option( BUILD_CLONE_SUBMODULES
        "Set this to ON to automatically clone any required Git submodules. When OFF, submodules must be manually cloned."
        ON )

include( CMakeDependentOption )
CMAKE_DEPENDENT_OPTION( BUILD_SHARED_LIBS
                        "Set this to ON to build all libraries as shared libraries. When OFF, libraries build as static libraries."
                        ON "${ALLOW_SHARED_LIBRARIES}"
                        OFF )

# Set output directories.
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin )
set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib )
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib )

# Include filepaths for source and include.
include( mqttFilePaths.cmake )

# MQTT library target.
add_library( mqtt
             ${MQTT_SOURCES} )

# MQTT public include path.
target_include_directories( mqtt PUBLIC ${MQTT_INCLUDE_PUBLIC_DIRS} )

# MQTT private include path.
target_include_directories( mqtt PRIVATE ${MQTT_INCLUDE_PRIVATE_DIRS} 
                                         ${MODULE_ROOT_DIR}/test )

# Organization of MQTT in IDE projects.
set_target_properties( mqtt PROPERTIES FOLDER libraries/standard )
source_group( include FILES source/include/mqtt.h source/include/mqtt_lightweight.h source/include/mqtt_state.h )
source_group( source FILES ${MQTT_SOURCES} )

# Include CMock build configuration.
include( test/cmock_build.cmake )

# Check if the CMock source directory exists, and if not present, clone the submodule
# if BUILD_CLONE_SUBMODULES configuration is enabled. 
if( NOT EXISTS ${MODULE_ROOT_DIR}/test/CMock/src )
    # Attempt to clone CMock.
    if( ${BUILD_CLONE_SUBMODULES} )
        clone_cmock()
    else()
        message( FATAL_ERROR "The required submodule CMock does not exist. Either clone it manually, or set BUILD_CLONE_SUBMODULES to 1 to automatically clone it during build." )
    endif()
endif()

# Add unit test and coverage configuration only if CMock directory exists.
if( EXISTS ${MODULE_ROOT_DIR}/test/CMock/src )

    # Add build targets for CMock and Unit, required for unit testing.
    add_cmock_targets()

    # Add function to enable CMOck based tests and coverage.
    include("${MODULE_ROOT_DIR}/tools/cmock/create_test.cmake")

    # Include build configuration for unit tests of MQTT.
    add_subdirectory( test )

    # Add a target for running coverage on tests.
    add_custom_target(coverage
        COMMAND ${CMAKE_COMMAND} -P ${CMAKE_SOURCE_DIR}/tools/cmock/coverage.cmake
        DEPENDS cmock unity mqtt_utest mqtt_lightweight_utest mqtt_state_utest
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    )
endif()
