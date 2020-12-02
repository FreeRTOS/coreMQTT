# Changelog for coreMQTT Client Library

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
