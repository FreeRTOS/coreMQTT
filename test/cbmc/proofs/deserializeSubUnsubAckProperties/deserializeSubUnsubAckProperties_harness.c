/*
 * coreMQTT
 * Copyright (C) 2020 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

/**
 * @file MQTT_DeserializePublish_harness.c
 * @brief Implements the proof harness for MQTT_DeserializePublish function.
 */

#include "core_mqtt.h"
#include "mqtt_cbmc_state.h"

/* Here we constraint the length of the properties to 25 bytes.
 */
#define MAX_PROPERTY_LENGTH         ( 300 )

#ifndef REMAINING_LENGTH_MAX
    #define REMAINING_LENGTH_MAX    CBMC_MAX_OBJECT_SIZE
#endif

void harness()
{
    MQTTPropBuilder_t * propBuffer;
    uint8_t * packetBytes;
    size_t propertyLength;
    uint32_t remainingLength;
    size_t minRemainingLength;

    propBuffer = allocateMqttPropBuilder( NULL );
    __CPROVER_assume( isValidMqttPropBuilder( propBuffer ) );

    __CPROVER_assume( propertyLength >= 0 );
    __CPROVER_assume( propertyLength <= MAX_PROPERTY_LENGTH );

    __CPROVER_assume( remainingLength >= 2U );
    __CPROVER_assume( remainingLength < REMAINING_LENGTH_MAX );

    packetBytes = malloc( remainingLength - sizeof( uint16_t ) );
    __CPROVER_assume( packetBytes != NULL );

    if( ( remainingLength - 2U ) < 1 )
    {
        /* Nothing to do - 0 length buffer. */
    }
    else if( ( remainingLength - 2U ) < 2 )
    {
        __CPROVER_assume( propertyLength < ( 128 ) );
        encodeVariableLength( packetBytes, propertyLength );
    }
    else
    {
        encodeVariableLength( packetBytes, propertyLength );
    }

    __CPROVER_file_local_core_mqtt_serializer_c_deserializeSubUnsubAckProperties( propBuffer,
                                                                                  packetBytes,
                                                                                  &propertyLength,
                                                                                  remainingLength );
}
