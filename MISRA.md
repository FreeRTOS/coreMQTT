# MISRA Compliance

The MQTT Client library files conform to the [MISRA C:2012](https://www.misra.org.uk)
guidelines, with some noted exceptions. Compliance is checked with Coverity static analysis.
The specific deviations, suppressed inline, are listed below.

Additionally, [MISRA configuration file](https://github.com/FreeRTOS/coreMQTT/blob/main/tools/coverity/misra.config) contains the project wide deviations.

### Suppressed with Coverity Comments
To find the deviation references in the source files run grep on the source code
with ( Assuming rule 5.4 violation; with justification in point 2 ):
```
grep 'MISRA Ref 5.4.2' . -rI
```

#### Rule 11.8
_Ref 11.8.1_

- MISRA Rule 11.8 flags casting away the const qualifier in the pointer
    type. This rule is suppressed because we are moving the base pointer
    of the vector forward by the amount of bytes sent. Firstly, we check
    that there is adequate data left in the vector. Then we decrement the
    amount of bytes from the length of the vector to ensure there are no issues.
