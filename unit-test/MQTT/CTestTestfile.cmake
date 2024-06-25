# CMake generated Testfile for
# Source directory: /home/ubuntu/coreMQTT/test/unit-test/MQTT
# Build directory: /home/ubuntu/coreMQTT/unit-test/MQTT
#
# This file includes the relevant testing commands required for
# testing this directory and lists subdirectories to be tested as well.
add_test([=[core_mqtt_utest]=] "/home/ubuntu/coreMQTT/bin/tests/core_mqtt_utest")
set_tests_properties([=[core_mqtt_utest]=] PROPERTIES  WORKING_DIRECTORY "/home/ubuntu/coreMQTT" _BACKTRACE_TRIPLES "/home/ubuntu/coreMQTT/tools/cmock/create_test.cmake;52;add_test;/home/ubuntu/coreMQTT/test/unit-test/MQTT/CMakeLists.txt;76;create_test;/home/ubuntu/coreMQTT/test/unit-test/MQTT/CMakeLists.txt;0;")
add_test([=[core_mqtt_state_utest]=] "/home/ubuntu/coreMQTT/bin/tests/core_mqtt_state_utest")
set_tests_properties([=[core_mqtt_state_utest]=] PROPERTIES  WORKING_DIRECTORY "/home/ubuntu/coreMQTT" _BACKTRACE_TRIPLES "/home/ubuntu/coreMQTT/tools/cmock/create_test.cmake;52;add_test;/home/ubuntu/coreMQTT/test/unit-test/MQTT/CMakeLists.txt;93;create_test;/home/ubuntu/coreMQTT/test/unit-test/MQTT/CMakeLists.txt;0;")
add_test([=[core_mqtt_serializer_utest]=] "/home/ubuntu/coreMQTT/bin/tests/core_mqtt_serializer_utest")
set_tests_properties([=[core_mqtt_serializer_utest]=] PROPERTIES  WORKING_DIRECTORY "/home/ubuntu/coreMQTT" _BACKTRACE_TRIPLES "/home/ubuntu/coreMQTT/tools/cmock/create_test.cmake;52;add_test;/home/ubuntu/coreMQTT/test/unit-test/MQTT/CMakeLists.txt;109;create_test;/home/ubuntu/coreMQTT/test/unit-test/MQTT/CMakeLists.txt;0;")
