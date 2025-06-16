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
 * @file MQTT_DeserializeAck_harness.c
 * @brief Implements the proof harness for MQTT_DeserializeAck function.
 */
#include "core_mqtt.h"
#include "mqtt_cbmc_state.h"

MQTTStatus_t __CPROVER_file_local_core_mqtt_serializer_c_deserializeConnackProperties( MQTTConnectionProperties_t * pConnackProperties,
                                                                                       size_t length,
                                                                                       uint8_t * pIndex,
                                                                                       MQTTPropBuilder_t * propBuffer )
{
    MQTTStatus_t status;

    return status;
}

MQTTStatus_t __CPROVER_file_local_core_mqtt_serializer_c_deserializeSubackProperties( MQTTPropBuilder_t * propBuffer,
                                                                                      uint8_t * pIndex,
                                                                                      size_t * pSubackPropertyLength,
                                                                                      size_t remainingLength )
{
    MQTTStatus_t status;

    return status;
}

MQTTStatus_t __CPROVER_file_local_core_mqtt_serializer_c_decodeAckProperties( MQTTPropBuilder_t * propBuffer,
                                                                              uint8_t * pIndex,
                                                                              size_t remainingLength )
{
    MQTTStatus_t status;

    return status;
}

void harness()
{
    MQTTPacketInfo_t * pIncomingPacket;
    uint16_t * pPacketId;
    bool * pSessionPresent;
    MQTTReasonCodeInfo_t * pReasonCode;
    MQTTPropBuilder_t * pPropertyBuilder;
    MQTTConnectionProperties_t * pConnectProperties;

    pIncomingPacket = allocateMqttPacketInfo( NULL );
    __CPROVER_assume( isValidMqttPacketInfo( pIncomingPacket ) );

    /* These are allocated for coverage of a NULL input. */
    pPacketId = malloc( sizeof( uint16_t ) );
    pSessionPresent = malloc( sizeof( bool ) );
    pReasonCode = malloc( sizeof( MQTTReasonCodeInfo_t ) );

    pPropertyBuilder = allocateMqttPropBuilder( NULL );
    __CPROVER_assume( isValidMqttPropBuilder( pPropertyBuilder ) );

    pConnectProperties = malloc( sizeof( MQTTConnectionProperties_t ) );

    MQTT_DeserializeAck( pIncomingPacket,
                         pPacketId,
                         pSessionPresent,
                         pReasonCode,
                         pPropertyBuilder,
                         pConnectProperties );
}
