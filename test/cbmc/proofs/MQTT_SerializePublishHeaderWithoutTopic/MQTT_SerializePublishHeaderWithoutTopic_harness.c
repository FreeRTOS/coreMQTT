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
 * @file MQTT_SerializePublishHeaderWithoutTopic_harness.c
 * @brief Implements the proof harness for MQTT_SerializePublishHeaderWithoutTopic function.
 */
#include "core_mqtt.h"
#include "mqtt_cbmc_state.h"

/* The maximum number of bytes MQTT_SerializePublishHeaderWithoutTopic() writes
 * into pBuffer is:
 *      1 (publish flags)
 *    + remainingLengthEncodedSize(remainingLength)   (at most 4)
 *    + 2 (topic name length high/low bytes)
 * The function only writes when remainingLength < MQTT_REMAINING_LENGTH_INVALID,
 * which bounds the encoded size at 4. Therefore a 7-byte buffer is always large
 * enough; it is the caller's responsibility (via MQTT_GetPublishPacketSize) to
 * provide a sufficiently sized buffer, so this models a conforming caller. */
#define HEADER_BUFFER_SIZE    ( 1U + 4U + 2U )

void harness()
{
    MQTTPublishInfo_t * pPublishInfo;
    uint32_t remainingLength;
    uint8_t * pBuffer;
    size_t * headerSize;

    pPublishInfo = allocateMqttPublishInfo( NULL );
    __CPROVER_assume( isValidMqttPublishInfo( pPublishInfo ) );

    /* It is part of the MQTT API contract that the topic name length fits in a
     * 16-bit field (it is serialized as a 2-byte UTF-8 string length). The
     * library enforces this with an assert on CHECK_SIZE_T_OVERFLOWS_16BIT in
     * serializePublishCommon(); model that conforming precondition here so the
     * UINT16_HIGH_BYTE() cast does not report a spurious conversion overflow. */
    if( pPublishInfo != NULL )
    {
        __CPROVER_assume( pPublishInfo->topicNameLength <= UINT16_MAX );
    }

    /* malloc() may return NULL to cover the NULL-parameter checks for pBuffer
     * and headerSize. */
    pBuffer = malloc( HEADER_BUFFER_SIZE );
    headerSize = malloc( sizeof( size_t ) );

    MQTT_SerializePublishHeaderWithoutTopic( pPublishInfo,
                                             remainingLength,
                                             pBuffer,
                                             headerSize );
}
