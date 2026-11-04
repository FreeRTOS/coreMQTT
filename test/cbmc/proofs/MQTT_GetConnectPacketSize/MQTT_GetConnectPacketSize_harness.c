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
 * @file MQTT_GetConnectPacketSize_harness.c
 * @brief Implements the proof harness for MQTT_GetConnectPacketSize function.
 */
#include "core_mqtt.h"
#include "mqtt_cbmc_state.h"

void harness()
{
    MQTTConnectInfo_t * pConnectInfo;
    MQTTPublishInfo_t * pWillInfo;
    MQTTPropBuilder_t * pConnectProperties;
    MQTTPropBuilder_t * pWillProperties;
    uint32_t * pRemainingLength;
    uint32_t * pPacketSize;

    pConnectInfo = allocateMqttConnectInfo( NULL );
    __CPROVER_assume( isValidMqttConnectInfo( pConnectInfo ) );

    pWillInfo = allocateMqttPublishInfo( NULL );
    __CPROVER_assume( isValidMqttPublishInfo( pWillInfo ) );

    pConnectProperties = allocateMqttPropBuilder( NULL );
    __CPROVER_assume( isValidMqttPropBuilder( pConnectProperties ) );

    pWillProperties = allocateMqttPropBuilder( NULL );
    __CPROVER_assume( isValidMqttPropBuilder( pWillProperties ) );

    /* The output parameters are plain pointers. They are allocated with malloc
    * so that they may also be NULL, which covers the NULL-parameter checks. */
    pRemainingLength = malloc( sizeof( uint32_t ) );
    pPacketSize = malloc( sizeof( uint32_t ) );

    MQTT_GetConnectPacketSize( pConnectInfo,
                               pWillInfo,
                               pConnectProperties,
                               pWillProperties,
                               pRemainingLength,
                               pPacketSize );
}
