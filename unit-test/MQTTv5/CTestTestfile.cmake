# CMake generated Testfile for 
# Source directory: /home/ubuntu/coreMQTT/test/unit-test/MQTTv5
# Build directory: /home/ubuntu/coreMQTT/unit-test/MQTTv5
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test([=[core_mqttv5_utest]=] "/home/ubuntu/coreMQTT/bin/tests/core_mqttv5_utest")
set_tests_properties([=[core_mqttv5_utest]=] PROPERTIES  WORKING_DIRECTORY "/home/ubuntu/coreMQTT" _BACKTRACE_TRIPLES "/home/ubuntu/coreMQTT/tools/cmock/create_test.cmake;52;add_test;/home/ubuntu/coreMQTT/test/unit-test/MQTTv5/CMakeLists.txt;77;create_test;/home/ubuntu/coreMQTT/test/unit-test/MQTTv5/CMakeLists.txt;0;")
add_test([=[core_mqttv5_state_utest]=] "/home/ubuntu/coreMQTT/bin/tests/core_mqttv5_state_utest")
set_tests_properties([=[core_mqttv5_state_utest]=] PROPERTIES  WORKING_DIRECTORY "/home/ubuntu/coreMQTT" _BACKTRACE_TRIPLES "/home/ubuntu/coreMQTT/tools/cmock/create_test.cmake;52;add_test;/home/ubuntu/coreMQTT/test/unit-test/MQTTv5/CMakeLists.txt;94;create_test;/home/ubuntu/coreMQTT/test/unit-test/MQTTv5/CMakeLists.txt;0;")
add_test([=[core_mqttv5_serializer_utest]=] "/home/ubuntu/coreMQTT/bin/tests/core_mqttv5_serializer_utest")
set_tests_properties([=[core_mqttv5_serializer_utest]=] PROPERTIES  WORKING_DIRECTORY "/home/ubuntu/coreMQTT" _BACKTRACE_TRIPLES "/home/ubuntu/coreMQTT/tools/cmock/create_test.cmake;52;add_test;/home/ubuntu/coreMQTT/test/unit-test/MQTTv5/CMakeLists.txt;110;create_test;/home/ubuntu/coreMQTT/test/unit-test/MQTTv5/CMakeLists.txt;0;")
