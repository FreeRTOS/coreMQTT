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
    remainingLengthForProperties -= ( pPublishInfo->qos > 0 ) ? sizeof( uint16_t ) : 0;

    status = __CPROVER_file_local_core_mqtt_serializer_c_decodeVariableLength( pLocalIndex, remainingLengthForProperties, &propertyLength );
    pPublishInfo->propertyLength = propertyLength;

    if( ( status == MQTTSuccess ) &&
        ( propertyLength > remainingLengthForProperties - variableLengthEncodedSizeForProof( propertyLength ) ) )
    {
        status = MQTTBadResponse;
    }

    return status;
}

void harness()
{
    MQTTPacketInfo_t * pIncomingPacket;
    MQTTPublishInfo_t * pPublishInfo;
    MQTTPropBuilder_t * propBuffer;
    uint16_t topicAliasMax;
    uint16_t * pPacketId;
    uint32_t maxPacketSize;

    pIncomingPacket = allocateMqttPacketInfo( NULL );
    __CPROVER_assume( isValidMqttPacketInfo( pIncomingPacket ) );

    pPublishInfo = allocateMqttPublishInfo( NULL );
    __CPROVER_assume( isValidMqttPublishInfo( pPublishInfo ) );

    propBuffer = allocateMqttPropBuilder( NULL );
    __CPROVER_assume( isValidMqttPropBuilder( propBuffer ) );

    pPacketId = malloc( sizeof( uint16_t ) );

    __CPROVER_assume( maxPacketSize > 0 );
    __CPROVER_assume( maxPacketSize < MQTT_MAX_PACKET_SIZE );

    /* This function grabs the topic name, the topic name length, the
     * the payload, and the payload length. */
    MQTT_DeserializePublish( pIncomingPacket, pPacketId, pPublishInfo, propBuffer, maxPacketSize, topicAliasMax );
}
