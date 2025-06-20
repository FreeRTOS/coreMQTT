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
 * @file MQTT_SerializeDisconnect_harness.c
 * @brief Implements the proof harness for MQTT_SerializeDisconnect function.
 */
#include "core_mqtt.h"
#include "mqtt_cbmc_state.h"

void harness()
{
    MQTTFixedBuffer_t * pFixedBuffer;
    MQTTPropBuilder_t * pDisconnectProperties;
    MQTTSuccessFailReasonCode_t reasonCode;
    MQTTStatus_t status;
    size_t remainingLength;
    size_t packetSize;
    uint32_t maxPacketSize;

    pDisconnectProperties = allocateMqttPropBuilder( NULL );

    if( pDisconnectProperties != NULL )
    {
        __CPROVER_assume( pDisconnectProperties->currentIndex >= 0 );
        __CPROVER_assume( pDisconnectProperties->currentIndex < pDisconnectProperties->bufferLength );
        __CPROVER_assume( pDisconnectProperties->fieldSet >= 0 );
    }

    pFixedBuffer = allocateMqttFixedBuffer( NULL );
    __CPROVER_assume( isValidMqttFixedBuffer( pFixedBuffer ) );

    status = MQTT_GetDisconnectPacketSize( pDisconnectProperties, &remainingLength, &packetSize, maxPacketSize, reasonCode );

    if( status == MQTTSuccess )
    {
        MQTT_SerializeDisconnect( pDisconnectProperties, reasonCode, remainingLength, pFixedBuffer );
    }
}
