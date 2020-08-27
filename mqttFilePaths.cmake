# This file is to add source files and include directories
# into variables so that it can be reused from different repositories
# in their Cmake based build system by including this file.
#
# Files specific to the repository such as test runner, platform tests
# are not added to the variables.

# MQTT library source files.
set( MQTT_SOURCES
     "${MODULES_DIR}/standard/mqtt/source/mqtt.c"
     "${MODULES_DIR}/standard/mqtt/source/mqtt_state.c" )

# Lightweight MQTT library source files.
set( LIGHTWEIGHT_MQTT_SOURCES
     "${MODULES_DIR}/standard/mqtt/source/mqtt_lightweight.c" )

# MQTT library Public Include directories.
set( MQTT_INCLUDE_PUBLIC_DIRS
     "${MODULES_DIR}/standard/mqtt/source/include"
     "${MODULES_DIR}/standard/mqtt/source/portable" )

# MQTT library Private Include directories.
set( MQTT_INCLUDE_PRIVATE_DIRS
     "${MODULES_DIR}/standard/mqtt/source" )