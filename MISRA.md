# MISRA Compliance

The MQTT library files conform to the [MISRA C:2012](https://www.misra.org.uk)
guidelines, with some noted exceptions. Compliance is checked with Coverity static analysis.
Deviations from the MISRA standard are listed below:

### Ignored by [Coverity Configuration](https://github.com/aws/aws-iot-device-sdk-embedded-C/blob/main/tools/coverity/misra.config)
| Deviation | Category | Justification |
| :-: | :-: | :-: |
| Directive 4.5 | Advisory | Allow names that MISRA considers ambiguous (such as LogInfo and LogError) |
| Directive 4.8 | Advisory | Allow inclusion of unused types. Header files for a specific port, which are needed by all files, may define types that are not used by a specific file. |
| Directive 4.9 | Advisory | Allow inclusion of function like macros. The `assert` macro is used throughout the library for parameter validation, and logging is done using function like macros. |
| Rule 2.3 | Advisory | Allow unused types. The `MQTTSubAckStatus_t` enum is unused in our source files, as it is intended for a user to use when parsing a subscription acknowledgment's response codes. |
| Rule 2.4 | Advisory | Allow unused tags. Some compilers warn if types are not tagged. |
| Rule 2.5 | Advisory | Allow unused macros. Library headers may define macros intended for the application's use, but are not used by a specific file. |
| Rule 3.1 | Required | Allow nested comments. C++ style `//` comments are used in example code within Doxygen documentation blocks. |
| Rule 11.5 | Advisory | Allow casts from `void *`. Fields such as publish payloads are passed as `void *` and must be cast to the correct data type before use. |

### Flagged by Coverity
| Deviation | Category | Justification |
| :-: | :-: | :-: |
| Rule 8.7 | Advisory | API functions are not used by the library outside of the files they are defined; however, they must be externally visible in order to be used by an application. |

### Suppressed with Coverity Comments
*None.*
