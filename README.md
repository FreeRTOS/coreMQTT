# MQTT Client Library

This repository contains an ISO C90 compliant MQTT client library designed for embedded platforms.

## Building Unit Tests.

### MQTT Config File

The MQTT client library exposes build configuration macros that are required for building the library.
A list of all the configurations and their default values are defined in [core_mqtt_config_defaults.h](https://github.com/FreeRTOS/coreMQTT/blob/master/source/include/core_mqtt_config_defaults.h). 
To provide custom values for the configuration macros, a custom config file named `core_mqtt_config.h` can be
provided to the library.

By default, a `core_mqtt_config.h` custom config is required to build the library. To disable this requirement
and build the library with default configuration values, provide `MQTT_DO_NOT_USE_CUSTOM_CONFIG` as a compile time preprocessor macro.

### Platform Prerequisites

- For building the library, CMake 3.13.0 or later and a C90 compiler.
- For running unit tests, Ruby 2.0.0 or later is additionally required for the CMock test framework (that we use).
- For running the coverage target, gcov is additionally required.

### Steps to build Unit Tests

1. Go to the root directory of this repository.

1. Create build directory: `mkdir build && cd build`

1. Run *cmake* while inside build directory: `cmake -S ../test/unit-test `

1. Run this command to build the library and unit tests: `make all`

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
