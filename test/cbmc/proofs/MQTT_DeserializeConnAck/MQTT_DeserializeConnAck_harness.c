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
 * @file MQTT_DeserializeConnAck_harness.c
 * @brief Implements the proof harness for MQTT_DeserializeConnAck function.
 *
 * MQTT_DeserializeConnAck() validates a CONNACK packet that was received from
 * the network (untrusted) and then hands the remaining data to the static
 * helper deserializeConnack(). The packet, its remaining-data buffer and its
 * remaining length are all modelled non-deterministically. The body of
 * deserializeConnack() is removed (REMOVE_FUNCTION_BODY) because its memory
 * safety is established by the DeserializeConnackProperties proof; this harness
 * verifies the wrapper's own NULL checks and length arithmetic.
 */

#include "core_mqtt.h"
#include "core_mqtt_serializer.h"
#include "private/core_mqtt_serializer_private.h"
#include "mqtt_cbmc_state.h"

/* The body of deserializeConnack() is removed (REMOVE_FUNCTION_BODY) and
 * replaced with this stub. Its memory safety is established by the
 * DeserializeConnackProperties proof, so here it is abstracted to a function
 * that returns a non-deterministic status without touching memory. */
MQTTStatus_t __CPROVER_file_local_core_mqtt_serializer_c_deserializeConnack( MQTTConnectionProperties_t * pConnackProperties,
                                                                             const MQTTPacketInfo_t * pIncomingPacket,
                                                                             bool * pSessionPresent,
                                                                             MQTTPropBuilder_t * pPropBuffer )
{
    MQTTStatus_t status;

    return status;
}

void harness()
{
    MQTTPacketInfo_t * pIncomingPacket;
    bool * pSessionPresent;
    MQTTPropBuilder_t * pPropBuffer;
    MQTTConnectionProperties_t * pConnectProperties;

    pIncomingPacket = allocateMqttPacketInfo( NULL );
    __CPROVER_assume( isValidMqttPacketInfo( pIncomingPacket ) );

    /* The MQTT remaining length is, by protocol, at most 268,435,455 and this
     * is enforced upstream by MQTT_GetIncomingPacketTypeAndLength(). The bound
     * keeps variableLengthEncodedSize()'s precondition satisfied. */
    if( pIncomingPacket != NULL )
    {
        __CPROVER_assume( pIncomingPacket->remainingLength <= MQTT_MAX_REMAINING_LENGTH );
    }

    /* Output parameter; may be NULL to exercise the NULL-parameter check. */
    pSessionPresent = malloc( sizeof( bool ) );

    pPropBuffer = allocateMqttPropBuilder( NULL );
    __CPROVER_assume( isValidMqttPropBuilder( pPropBuffer ) );

    /* May be NULL to exercise the NULL-parameter check. */
    pConnectProperties = malloc( sizeof( MQTTConnectionProperties_t ) );

    MQTT_DeserializeConnAck( pIncomingPacket,
                             pSessionPresent,
                             pPropBuffer,
                             pConnectProperties );
}
