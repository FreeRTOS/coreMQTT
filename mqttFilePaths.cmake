# This file is to add source files and include directories
# into variables so that it can be reused from different repositories
# in their Cmake based build system by including this file.
#
# Files specific to the repository such as test runner, platform tests
# are not added to the variables.

# MQTT library source files.
set( MQTT_SOURCES
     "${CMAKE_CURRENT_LIST_DIR}/source/core_mqtt.c"
     "${CMAKE_CURRENT_LIST_DIR}/source/core_mqtt_state.c" )

# Lightweight MQTT library source files.
set( LIGHTWEIGHT_MQTT_SOURCES
     "${CMAKE_CURRENT_LIST_DIR}/source/core_mqtt_lightweight.c" )

# MQTT library Public Include directories.
set( MQTT_INCLUDE_PUBLIC_DIRS
     "${CMAKE_CURRENT_LIST_DIR}/source/include"
     "${CMAKE_CURRENT_LIST_DIR}/source/portable" )

# MQTT library Private Include directories.
set( MQTT_INCLUDE_PRIVATE_DIRS
     "${CMAKE_CURRENT_LIST_DIR}/source" )
