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
 * @file MQTT_Disconnect_harness.c
 * @brief Implements the proof harness for MQTT_Disconnect function.
 */
#include "core_mqtt.h"
#include "mqtt_cbmc_state.h"

void harness()
{
    MQTTVec_t * mqttVec;
    size_t memoryRequired;
    uint8_t * memoryBuffer;

    mqttVec = allocateMqttVec( NULL );

    memoryRequired = MQTT_GetBytesInMQTTVec( mqttVec );

    /* It is a part of the API contract that #MQTT_SerializeMQTTVec will be called with
     * a memory buffer of size output by #MQTT_GetBytesInMQTTVec function and the
     * #MQTTVec_t pointer given by the library as an input to the user defined
     * #MQTTStorePacketForRetransmit callback function. Hence the memory buffer must
     * not be NULL.
     */
    memoryBuffer = malloc( memoryRequired );
    __CPROVER_assume( memoryBuffer != NULL );

    MQTT_SerializeMQTTVec( memoryBuffer, mqttVec );
}
