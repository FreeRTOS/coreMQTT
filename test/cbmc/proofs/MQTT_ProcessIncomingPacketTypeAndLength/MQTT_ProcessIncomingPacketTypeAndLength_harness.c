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
 * @file MQTT_ProcessIncomingPacketTypeAndLength_harness.c
 * @brief Implements the proof harness for MQTT_ProcessIncomingPacketTypeAndLength function.
 *
 * MQTT_ProcessIncomingPacketTypeAndLength() interprets the first byte (packet
 * type) and the variable-length "remaining length" field directly out of a
 * caller-supplied buffer that was filled from the network. Both the buffer
 * contents and the number of available bytes (*pIndex) are untrusted, so they
 * are modelled non-deterministically here. The harness proves the function is
 * memory safe for any such input.
 */

#include "core_mqtt.h"
#include "core_mqtt_serializer.h"
#include "mqtt_cbmc_state.h"

void harness()
{
    uint8_t * pBuffer;
    size_t * pIndex;
    size_t bufferSize;
    MQTTPacketInfo_t * pIncomingPacket;

    /* The buffer holding the bytes received from the network. It may be NULL
     * (to exercise the NULL-parameter check) or of any size. */
    pBuffer = malloc( bufferSize );

    /* pIndex points at the count of bytes currently available in pBuffer. It
     * may be NULL to exercise the NULL-parameter check. */
    pIndex = malloc( sizeof( size_t ) );

    /* The function reads pBuffer[ 0 ] when *pIndex >= 1 and, inside
     * processRemainingLength(), pBuffer[ bytesDecoded + 1 ] only while
     * *pIndex > ( bytesDecoded + 1 ). Constraining the number of available
     * bytes to the actual allocation size is what a correct caller guarantees
     * and is required for the reads to stay in bounds. */
    if( ( pBuffer != NULL ) && ( pIndex != NULL ) )
    {
        __CPROVER_assume( *pIndex <= bufferSize );
    }

    pIncomingPacket = allocateMqttPacketInfo( NULL );
    __CPROVER_assume( isValidMqttPacketInfo( pIncomingPacket ) );

    MQTT_ProcessIncomingPacketTypeAndLength( pBuffer, pIndex, pIncomingPacket );
}
