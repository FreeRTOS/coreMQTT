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
 * @file event_callback_stub.c
 * @brief A stub for the event callback.
 */

#include <stdint.h>
#include "core_mqtt.h"

static size_t variableLengthEncodedSizeForProof( size_t length )
{
    size_t encodedSize;

    /* Determine how many bytes are needed to encode length.
     * The values below are taken from the MQTT 3.1.1 spec. */

    /* 1 byte is needed to encode lengths between 0 and 127. */
    if( length < 128U )
    {
        encodedSize = 1U;
    }
    /* 2 bytes are needed to encode lengths between 128 and 16,383. */
    else if( length < 16384U )
    {
        encodedSize = 2U;
    }
    /* 3 bytes are needed to encode lengths between 16,384 and 2,097,151. */
    else if( length < 2097152U )
    {
        encodedSize = 3U;
    }
    /* 4 bytes are needed to encode lengths between 2,097,152 and 268,435,455. */
    else
    {
        encodedSize = 4U;
    }

    LogDebug( ( "Encoded size for length %lu is %lu bytes.",
                ( unsigned long ) length,
                ( unsigned long ) encodedSize ) );

    return encodedSize;
}

MQTTStatus_t __CPROVER_file_local_core_mqtt_serializer_c_deserializePublishProperties( MQTTPublishInfo_t * pPublishInfo,
                                                  MQTTPropBuilder_t * propBuffer,
                                                  uint8_t * pIndex,
                                                  uint16_t topicAliasMax,
                                                  size_t remainingLength )
{
    MQTTStatus_t status;
    size_t propertyLength = 0U;
    uint8_t * pLocalIndex = pIndex;
    size_t remainingLengthForProperties;

    remainingLengthForProperties = remainingLength;
    remainingLengthForProperties -= pPublishInfo->topicNameLength + sizeof( uint16_t );
    remainingLengthForProperties -= ( pPublishInfo->qos > 0 )? sizeof( uint16_t ) : 0;

    status = __CPROVER_file_local_core_mqtt_serializer_c_decodeVariableLength(pLocalIndex, remainingLengthForProperties, &propertyLength); 
    pPublishInfo->propertyLength = propertyLength;

    if( status == MQTTSuccess &&
        propertyLength > remainingLengthForProperties - variableLengthEncodedSizeForProof( propertyLength ) )
    {
        status = MQTTBadResponse;
    }

    return status;
}