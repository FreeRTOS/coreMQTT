# coreMQTT Client Library

This repository contains the coreMQTT library that has been optimized for a low memory footprint.  The coreMQTT library is fully compliant with the [MQTT 3.1.1](http://docs.oasis-open.org/mqtt/mqtt/v3.1.1/mqtt-v3.1.1.html) standard.  It has no dependencies on any additional libraries other than the standard C library, a customer-implemented network transport interface, and *optionally* a user-implemented platform time function.  This library is distributed under the [MIT Open Source License](LICENSE).

This library has gone through code quality checks including for [GNU Complexity](https://www.gnu.org/software/complexity/manual/complexity.html) and [MISRA coding standard compl](https://www.misra.org.uk/MISRAHome/MISRAC2012/tabid/196/Default.aspx).  Deviations to the MISRA C:2012 guidelines are documented under [MISRA Deviations](MISRA.md). This library has also undergone both static code analysis from [Coverity statical analysis](https://scan.coverity.com/) and validation of memory safety and functional correctness proof through the [AWS CBMC automated reasoning tool](https://www.youtube.com/watch?v=YwQHAPRhQkI&feature=youtu.be&t=1721).  

## Building Unit Tests

### MQTT Config File

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

### Platform Prerequisites

- For building the library, **CMake 3.13.0 or later** and a **C90 compiler**.
- For running unit tests, Ruby 2.0.0 or later is additionally required for the CMock test framework (that we use).
- For running the coverage target, gcov is additionally required.

### Steps to build **Library** and **Unit Tests**

1. Go to the root directory of this repository.

1. Run *cmake* while inside build directory: `cmake -S ../test -B build -DBUILD_CLONE_SUBMODULES=ON `

1. Run this command to build the library and unit tests: `make -C build all`

1. The built library will be present in `build/lib` folder, and generated test executables will be present in `build/bin/tests` folder.

1. Run `ctest` to execute all tests and view the test run summary.

## Reference examples

The AWS IoT Embedded C-SDK repository contains demos of using the MQTT client library [here](https://github.com/aws/aws-iot-device-sdk-embedded-C/tree/development/demos/mqtt) on a POSIX platform. These can be used as reference examples for the library API.

## Generating documentation

The Doxygen references were created using Doxygen version 1.8.20. To generate the
Doxygen pages, please run the following command from the root of this repository:

```shell
doxygen docs/doxygen/config.doxyfile
```
