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

MQTTStatus_t __CPROVER_file_local_core_mqtt_serializer_c_deserializeDisconnectProperties( uint8_t * pIndex,
                                                                                          size_t propertyLength )
{
    MQTTStatus_t status;

    return status;
}

void harness()
{
    MQTTReasonCodeInfo_t * pDisconnectInfo;
    MQTTPacketInfo_t * pIncomingPacket;
    MQTTPropBuilder_t * propBuffer;
    uint32_t maxPacketSize;

    pIncomingPacket = allocateMqttPacketInfo( NULL );
    __CPROVER_assume( isValidMqttPacketInfo( pIncomingPacket ) );

    propBuffer = allocateMqttPropBuilder( NULL );
    __CPROVER_assume( isValidMqttPropBuilder( propBuffer ) );

    __CPROVER_assume( maxPacketSize > 0 );
    __CPROVER_assume( maxPacketSize < MQTT_MAX_PACKET_SIZE );

    pDisconnectInfo = malloc( sizeof( MQTTReasonCodeInfo_t ) );

    /* This function grabs the topic name, the topic name length, the
     * the payload, and the payload length. */
    MQTT_DeserializeDisconnect( pIncomingPacket, maxPacketSize, pDisconnectInfo, propBuffer );
}
