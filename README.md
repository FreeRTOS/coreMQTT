# MQTT Client Library

This repository contains an ISO C90 compliant MQTT client library designed for embedded platforms.

## MQTT Config File

The MQTT client library exposes build configuration macros that are required for building the library.
A list of all the configurations and their default values are defined in [core_mqtt_config_defaults.h](https://github.com/FreeRTOS/coreMQTT/blob/master/source/include/core_mqtt_config_defaults.h). 
To provide custom values for the configuration macros, a custom config file named `core_mqtt_config.h` can be
provided by the application to the library.

By default, a `core_mqtt_config.h` custom config is required to build the library. To disable this requirement
and build the library with default configuration values, provide `MQTT_DO_NOT_USE_CUSTOM_CONFIG` as a compile time preprocessor macro.

**Thus, the MQTT library can be built by either**:
* Defining a `core_mqtt_config.h` file in the application, and adding it to the include directories list of the library  
**OR**
* Defining the `MQTT_DO_NOT_USE_CUSTOM_CONFIG` preprocessor macro for the library build.

## Building Library

The [mqttFilePaths.cmake](https://github.com/FreeRTOS/coreMQTT/blob/master/mqttFilePaths.cmake) file contains the information of all source files and the header include path required to build the NQTT library.

As mentioned in the previous section, either a custom config file (i.e. `core_mqtt_config.h`) OR `MQTT_DO_NOT_USE_CUSTOM_CONFIG` macro needs to be provided to build the MQTT library.

For a CMake example of building the MQTT library with the `mqttFilePaths.cmake` file, refer to the `coverity_analysis` library target in [test/CMakeLists.txt](https://github.com/FreeRTOS/coreMQTT/blob/master/test/CMakeLists.txt) file.

## Building Unit Tests

### Platform Prerequisites

- For building the library, **CMake 3.13.0 or later** and a **C90 compiler**.
- For running unit tests, **Ruby 2.0.0** or later is additionally required for the CMock test framework (that we use).
- For running the coverage target, **gcov** and **lcov** are additionally required.

### Steps to build **Unit Tests**

1. Go to the root directory of this repository.

1. Run the *cmake* command: `cmake -S ../test -B build -DBUILD_CLONE_SUBMODULES=ON `

1. Run this command to build the library and unit tests: `make -C build all`

1. The generated test executables will be present in `build/bin/tests` folder.

1. Run `ctest` to execute all tests and view the test run summary.

## Reference examples

The AWS IoT Embedded C-SDK repository contains demos of using the MQTT client library [here](https://github.com/aws/aws-iot-device-sdk-embedded-C/tree/development/demos/mqtt) on a POSIX platform. These can be used as reference examples for the library API.

## Generating documentation

The Doxygen references were created using Doxygen version 1.8.20. To generate the
Doxygen pages, please run the following command from the root of this repository:

```shell
doxygen docs/doxygen/config.doxyfile
```
