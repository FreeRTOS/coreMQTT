# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.30

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /home/ubuntu/cmake-3.30.0-rc3-linux-x86_64/bin/cmake

# The command to remove a file.
RM = /home/ubuntu/cmake-3.30.0-rc3-linux-x86_64/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/ubuntu/coreMQTT/test

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/ubuntu/coreMQTT

# Include any dependencies generated for this target.
include unit-test/MQTTv5/CMakeFiles/core_mqttv5_mock.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include unit-test/MQTTv5/CMakeFiles/core_mqttv5_mock.dir/compiler_depend.make

# Include the progress variables for this target.
include unit-test/MQTTv5/CMakeFiles/core_mqttv5_mock.dir/progress.make

# Include the compile flags for this target's objects.
include unit-test/MQTTv5/CMakeFiles/core_mqttv5_mock.dir/flags.make

unit-test/MQTTv5/mocks/mock_core_mqtt_serializer.c:
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --blue --bold --progress-dir=/home/ubuntu/coreMQTT/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Generating mocks/mock_core_mqtt_serializer.c"
	cd /home/ubuntu/coreMQTT/unit-test/MQTTv5 && ruby /home/ubuntu/coreMQTT/test/unit-test/CMock/lib/cmock.rb -o/home/ubuntu/coreMQTT/tools/cmock/project.yml /home/ubuntu/coreMQTT/source/include/core_mqtt_serializer.h

unit-test/MQTTv5/mocks/mock_core_mqtt_state.c:
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --blue --bold --progress-dir=/home/ubuntu/coreMQTT/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Generating mocks/mock_core_mqtt_state.c"
	cd /home/ubuntu/coreMQTT/unit-test/MQTTv5 && ruby /home/ubuntu/coreMQTT/test/unit-test/CMock/lib/cmock.rb -o/home/ubuntu/coreMQTT/tools/cmock/project.yml /home/ubuntu/coreMQTT/source/include/core_mqtt_state.h

unit-test/MQTTv5/CMakeFiles/core_mqttv5_mock.dir/mocks/mock_core_mqtt_serializer.c.o: unit-test/MQTTv5/CMakeFiles/core_mqttv5_mock.dir/flags.make
unit-test/MQTTv5/CMakeFiles/core_mqttv5_mock.dir/mocks/mock_core_mqtt_serializer.c.o: unit-test/MQTTv5/mocks/mock_core_mqtt_serializer.c
unit-test/MQTTv5/CMakeFiles/core_mqttv5_mock.dir/mocks/mock_core_mqtt_serializer.c.o: unit-test/MQTTv5/CMakeFiles/core_mqttv5_mock.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/ubuntu/coreMQTT/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object unit-test/MQTTv5/CMakeFiles/core_mqttv5_mock.dir/mocks/mock_core_mqtt_serializer.c.o"
	cd /home/ubuntu/coreMQTT/unit-test/MQTTv5 && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT unit-test/MQTTv5/CMakeFiles/core_mqttv5_mock.dir/mocks/mock_core_mqtt_serializer.c.o -MF CMakeFiles/core_mqttv5_mock.dir/mocks/mock_core_mqtt_serializer.c.o.d -o CMakeFiles/core_mqttv5_mock.dir/mocks/mock_core_mqtt_serializer.c.o -c /home/ubuntu/coreMQTT/unit-test/MQTTv5/mocks/mock_core_mqtt_serializer.c

unit-test/MQTTv5/CMakeFiles/core_mqttv5_mock.dir/mocks/mock_core_mqtt_serializer.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/core_mqttv5_mock.dir/mocks/mock_core_mqtt_serializer.c.i"
	cd /home/ubuntu/coreMQTT/unit-test/MQTTv5 && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/ubuntu/coreMQTT/unit-test/MQTTv5/mocks/mock_core_mqtt_serializer.c > CMakeFiles/core_mqttv5_mock.dir/mocks/mock_core_mqtt_serializer.c.i

unit-test/MQTTv5/CMakeFiles/core_mqttv5_mock.dir/mocks/mock_core_mqtt_serializer.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/core_mqttv5_mock.dir/mocks/mock_core_mqtt_serializer.c.s"
	cd /home/ubuntu/coreMQTT/unit-test/MQTTv5 && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/ubuntu/coreMQTT/unit-test/MQTTv5/mocks/mock_core_mqtt_serializer.c -o CMakeFiles/core_mqttv5_mock.dir/mocks/mock_core_mqtt_serializer.c.s

unit-test/MQTTv5/CMakeFiles/core_mqttv5_mock.dir/mocks/mock_core_mqtt_state.c.o: unit-test/MQTTv5/CMakeFiles/core_mqttv5_mock.dir/flags.make
unit-test/MQTTv5/CMakeFiles/core_mqttv5_mock.dir/mocks/mock_core_mqtt_state.c.o: unit-test/MQTTv5/mocks/mock_core_mqtt_state.c
unit-test/MQTTv5/CMakeFiles/core_mqttv5_mock.dir/mocks/mock_core_mqtt_state.c.o: unit-test/MQTTv5/CMakeFiles/core_mqttv5_mock.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/ubuntu/coreMQTT/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building C object unit-test/MQTTv5/CMakeFiles/core_mqttv5_mock.dir/mocks/mock_core_mqtt_state.c.o"
	cd /home/ubuntu/coreMQTT/unit-test/MQTTv5 && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT unit-test/MQTTv5/CMakeFiles/core_mqttv5_mock.dir/mocks/mock_core_mqtt_state.c.o -MF CMakeFiles/core_mqttv5_mock.dir/mocks/mock_core_mqtt_state.c.o.d -o CMakeFiles/core_mqttv5_mock.dir/mocks/mock_core_mqtt_state.c.o -c /home/ubuntu/coreMQTT/unit-test/MQTTv5/mocks/mock_core_mqtt_state.c

unit-test/MQTTv5/CMakeFiles/core_mqttv5_mock.dir/mocks/mock_core_mqtt_state.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/core_mqttv5_mock.dir/mocks/mock_core_mqtt_state.c.i"
	cd /home/ubuntu/coreMQTT/unit-test/MQTTv5 && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/ubuntu/coreMQTT/unit-test/MQTTv5/mocks/mock_core_mqtt_state.c > CMakeFiles/core_mqttv5_mock.dir/mocks/mock_core_mqtt_state.c.i

unit-test/MQTTv5/CMakeFiles/core_mqttv5_mock.dir/mocks/mock_core_mqtt_state.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/core_mqttv5_mock.dir/mocks/mock_core_mqtt_state.c.s"
	cd /home/ubuntu/coreMQTT/unit-test/MQTTv5 && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/ubuntu/coreMQTT/unit-test/MQTTv5/mocks/mock_core_mqtt_state.c -o CMakeFiles/core_mqttv5_mock.dir/mocks/mock_core_mqtt_state.c.s

# Object files for target core_mqttv5_mock
core_mqttv5_mock_OBJECTS = \
"CMakeFiles/core_mqttv5_mock.dir/mocks/mock_core_mqtt_serializer.c.o" \
"CMakeFiles/core_mqttv5_mock.dir/mocks/mock_core_mqtt_state.c.o"

# External object files for target core_mqttv5_mock
core_mqttv5_mock_EXTERNAL_OBJECTS =

unit-test/MQTTv5/lib/libcore_mqttv5_mock.so: unit-test/MQTTv5/CMakeFiles/core_mqttv5_mock.dir/mocks/mock_core_mqtt_serializer.c.o
unit-test/MQTTv5/lib/libcore_mqttv5_mock.so: unit-test/MQTTv5/CMakeFiles/core_mqttv5_mock.dir/mocks/mock_core_mqtt_state.c.o
unit-test/MQTTv5/lib/libcore_mqttv5_mock.so: unit-test/MQTTv5/CMakeFiles/core_mqttv5_mock.dir/build.make
unit-test/MQTTv5/lib/libcore_mqttv5_mock.so: lib/libcmock.a
unit-test/MQTTv5/lib/libcore_mqttv5_mock.so: lib/libunity.a
unit-test/MQTTv5/lib/libcore_mqttv5_mock.so: unit-test/MQTTv5/CMakeFiles/core_mqttv5_mock.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/home/ubuntu/coreMQTT/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Linking C shared library lib/libcore_mqttv5_mock.so"
	cd /home/ubuntu/coreMQTT/unit-test/MQTTv5 && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/core_mqttv5_mock.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
unit-test/MQTTv5/CMakeFiles/core_mqttv5_mock.dir/build: unit-test/MQTTv5/lib/libcore_mqttv5_mock.so
.PHONY : unit-test/MQTTv5/CMakeFiles/core_mqttv5_mock.dir/build

unit-test/MQTTv5/CMakeFiles/core_mqttv5_mock.dir/clean:
	cd /home/ubuntu/coreMQTT/unit-test/MQTTv5 && $(CMAKE_COMMAND) -P CMakeFiles/core_mqttv5_mock.dir/cmake_clean.cmake
.PHONY : unit-test/MQTTv5/CMakeFiles/core_mqttv5_mock.dir/clean

unit-test/MQTTv5/CMakeFiles/core_mqttv5_mock.dir/depend: unit-test/MQTTv5/mocks/mock_core_mqtt_serializer.c
unit-test/MQTTv5/CMakeFiles/core_mqttv5_mock.dir/depend: unit-test/MQTTv5/mocks/mock_core_mqtt_state.c
	cd /home/ubuntu/coreMQTT && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/ubuntu/coreMQTT/test /home/ubuntu/coreMQTT/test/unit-test/MQTTv5 /home/ubuntu/coreMQTT /home/ubuntu/coreMQTT/unit-test/MQTTv5 /home/ubuntu/coreMQTT/unit-test/MQTTv5/CMakeFiles/core_mqttv5_mock.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : unit-test/MQTTv5/CMakeFiles/core_mqttv5_mock.dir/depend
