# MQTT Client Library

This repository contains an ISO C89/C90 compliant MQTT client library designed for embedded platforms.

## Building Library and Unit Tests.

### Prerequisites

- CMake 3.13.0 or later and a C90 compiler.

- A supported operating system. The ports provided with this repo are expected to work with all recent versions of the following operating systems, although we cannot guarantee the behavior on all systems.

## Build Steps

1. Go to the root directory of this repository.

1. Create build directory: `mkdir build && cd build`

1. Run *cmake* while inside build directory: `cmake .. `

1. Run this command to build the library and unit tests: `make all`

1. The library will be present in `build/lib`, and test executables will be present in `build/bin/tests`.

1. Run `ctest` to run all tests and view the result summary.

## Generating documentation

The Doxygen references were created using Doxygen version 1.8.20. To generate the
Doxygen pages, please run the following command from the root of this repository:

```shell
doxygen docs/doxygen/config.doxyfile
```
