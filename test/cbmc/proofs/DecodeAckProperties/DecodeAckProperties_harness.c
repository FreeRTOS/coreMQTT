/*
 * coreMQTT <DEVELOPMENT BRANCH>
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
#define MAX_PROPERTY_LENGTH                           25U

/* Here we assume the minimum size of a property can only be for a variable length
 * integer property, e.g. subscription ID. Those will contain a 1 byte property ID,
 * and a variable length integer. Due to this the maximum number of properties that
 * will be in the packet will be MAX_PROPERTY_LENGTH / MIN_LENGTH_OF_SINGLE_PROPERTY.
 */
#define MIN_LENGTH_OF_SINGLE_PROPERTY                 ( 2U )

#define MIN_REMAINING_LENGTH_FOR_ACK_WITHOUT_PROPS    ( 2U )

#ifndef REMAINING_LENGTH_MAX
    #define REMAINING_LENGTH_MAX                      CBMC_MAX_OBJECT_SIZE
#endif

void harness()
{
    MQTTPropBuilder_t * propBuffer;
    uint8_t * packetBytes;
    size_t remainingLength;
    size_t propertyLength;

    propBuffer = allocateMqttPropBuilder( NULL );
    __CPROVER_assume( isValidMqttPropBuilder( propBuffer ) );

    __CPROVER_assume( propertyLength >= 0 );
    __CPROVER_assume( propertyLength <= MAX_PROPERTY_LENGTH );

    remainingLength = MIN_REMAINING_LENGTH_FOR_ACK_WITHOUT_PROPS;
    remainingLength += variableLengthEncodedSizeForProof( propertyLength ) + propertyLength;

    packetBytes = malloc( remainingLength );
    __CPROVER_assume( packetBytes != NULL );

    encodeVariableLength( &packetBytes[ MIN_REMAINING_LENGTH_FOR_ACK_WITHOUT_PROPS ], propertyLength );

    __CPROVER_file_local_core_mqtt_serializer_c_decodeAckProperties( propBuffer, packetBytes, remainingLength );
}
