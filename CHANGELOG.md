# Changelog for coreMQTT Client Library

## v1.1.0 (December 2020)

### Updates
 - [#118](https://github.com/FreeRTOS/coreMQTT/pull/118) Use the `stdbool.h` header file instead of using preprocessor checks for when `bool` is not defined. This also provides `stdbool.readme` and `stdint.readme` files in the case that a non-C99 compiler does not provide the respective header.
 - [#120](https://github.com/FreeRTOS/coreMQTT/pull/120) Introduce a `MQTT_RECV_POLLING_TIMEOUT_MS` configuration macro to control the timeout for retrying zero byte network read operations. Previously, network read attempts were controlled by a runtime timeout parameter, which could result in a premature timeout even when data could still be read. Now, reads will wait for at least the macro timeout value before returning error. Conversely, the macro timeout value is now the maximum duration during which no data may be received, regardless of the timeout passed at runtime.
 - [#124](https://github.com/FreeRTOS/coreMQTT/pull/124), [#127](https://github.com/FreeRTOS/coreMQTT/pull/127), Introduce a `MQTT_SEND_RETRY_TIMEOUT_MS` configuration macro to control the similar case of retrying zero byte transport send operations.
 - [#139](https://github.com/FreeRTOS/coreMQTT/pull/139) Add a parameter check for empty topic filters in SUBSCRIBE and UNSUBSCRIBE packets.

### Other
 - [#107](https://github.com/FreeRTOS/coreMQTT/pull/107), [#109](https://github.com/FreeRTOS/coreMQTT/pull/109), [#121](https://github.com/FreeRTOS/coreMQTT/pull/121) Improve continuous integration checks.
 - [#110](https://github.com/FreeRTOS/coreMQTT/pull/107) Rename the master branch to main.
 - [#113](https://github.com/FreeRTOS/coreMQTT/pull/113), [#116](https://github.com/FreeRTOS/coreMQTT/pull/116), [#117](https://github.com/FreeRTOS/coreMQTT/pull/117), [#125](https://github.com/FreeRTOS/coreMQTT/pull/125) Update logging and cast to C standard types for logs.
 - [#115](https://github.com/FreeRTOS/coreMQTT/pull/115), [#122](https://github.com/FreeRTOS/coreMQTT/pull/122), [#128](https://github.com/FreeRTOS/coreMQTT/pull/128), [#132](https://github.com/FreeRTOS/coreMQTT/pull/132), [#133](https://github.com/FreeRTOS/coreMQTT/pull/133), [#136](https://github.com/FreeRTOS/coreMQTT/pull/136), [#138](https://github.com/FreeRTOS/coreMQTT/pull/138) Minor documentation updates.

## v1.0.1 (November 2020)

### Updates
 - [#83](https://github.com/FreeRTOS/coreMQTT/pull/83) Accept duplicate publishes regardless of the value of the "DUP" flag.
 - [#86](https://github.com/FreeRTOS/coreMQTT/pull/86) Remove `const` qualifier from transport interface function pointers.
 - [#91](https://github.com/FreeRTOS/coreMQTT/pull/91) `transport_interface.h` was moved to the `interface/` directory.

### Other
 - [#69](https://github.com/FreeRTOS/coreMQTT/pull/69), [#80](https://github.com/FreeRTOS/coreMQTT/pull/80), [#95](https://github.com/FreeRTOS/coreMQTT/pull/95), [#98](https://github.com/FreeRTOS/coreMQTT/pull/98) Minor documentation updates.
 - [#71](https://github.com/FreeRTOS/coreMQTT/pull/71) Set publish payloads to NULL when they are zero length.
 - [#74](https://github.com/FreeRTOS/coreMQTT/pull/74) Resolve clang build warnings from the unit tests.
 - [#75](https://github.com/FreeRTOS/coreMQTT/pull/75) Configure submodules to not be cloned by default.

## v1.0.0 (September 2020)

This is the first release of a coreMQTT client library in this repository.

The MQTT library is a client-side implementation that is compliant with the [MQTT 3.1.1 specification](http://docs.oasis-open.org/mqtt/mqtt/v3.1.1/os/mqtt-v3.1.1-os.html).   It is optimized for resource-constrained devices, and does not allocate any memory.
