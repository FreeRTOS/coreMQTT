cmake_minimum_required(VERSION 3.15)

project(coreMQTT LANGUAGES C)

# ------------------------------------------------------------------------------
# Includes
# ------------------------------------------------------------------------------

include(${CMAKE_CURRENT_LIST_DIR}/mqttFilePaths.cmake)

# ------------------------------------------------------------------------------
# Library targets
# ------------------------------------------------------------------------------

add_library( core_mqtt )


target_sources( core_mqtt PUBLIC ${MQTT_SOURCES}
                                 ${MQTT_SERIALIZER_SOURCES} )

target_include_directories( core_mqtt PUBLIC ${MQTT_INCLUDE_PUBLIC_DIRS} )

if(STANDALONE_TEST_BUILD_UNIX)
    target_compile_definitions( core_mqtt PUBLIC MQTT_DO_NOT_USE_CUSTOM_CONFIG=1 )
endif()
