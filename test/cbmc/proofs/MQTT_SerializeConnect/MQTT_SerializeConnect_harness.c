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
 * @file MQTT_SerializeConnect_harness.c
 * @brief Implements the proof harness for MQTT_SerializeConnect function.
 */
#include "core_mqtt.h"
#include "mqtt_cbmc_state.h"

/* Here we constraint the length of the properties to 25 bytes.
 */
#define MAX_PROPERTY_LENGTH              ( 25U )

void harness()
{
    MQTTConnectInfo_t * pConnectInfo;
    MQTTPublishInfo_t * pWillInfo;
    size_t remainingLength;
    MQTTFixedBuffer_t * pFixedBuffer;
    size_t packetSize;
    MQTTStatus_t status = MQTTSuccess;
    MQTTPropBuilder_t * pConnectProperties;
    MQTTPropBuilder_t * pWillProperties;

    pConnectInfo = allocateMqttConnectInfo( NULL );
    __CPROVER_assume( isValidMqttConnectInfo( pConnectInfo ) );

    pWillInfo = allocateMqttPublishInfo( NULL );
    __CPROVER_assume( isValidMqttPublishInfo( pWillInfo ) );

    pFixedBuffer = allocateMqttFixedBuffer( NULL );
    __CPROVER_assume( isValidMqttFixedBuffer( pFixedBuffer ) );

    pConnectProperties = allocateMqttPropBuilder( NULL );

    if( pConnectProperties != NULL )
    {
        __CPROVER_assume( pConnectProperties->bufferLength >= 0 );
        __CPROVER_assume( pConnectProperties->bufferLength <= MAX_PROPERTY_LENGTH );
        __CPROVER_assume( pConnectProperties->currentIndex >= 0 );
        __CPROVER_assume( pConnectProperties->currentIndex < pConnectProperties->bufferLength );
        __CPROVER_assume( pConnectProperties->fieldSet >= 0 );
    }

    pWillProperties = allocateMqttPropBuilder( NULL );

    if( pWillProperties != NULL )
    {
        __CPROVER_assume( pWillProperties->bufferLength >= 0 );
        __CPROVER_assume( pWillProperties->bufferLength <= MAX_PROPERTY_LENGTH );
        __CPROVER_assume( pWillProperties->currentIndex >= 0 );
        __CPROVER_assume( pWillProperties->currentIndex < pWillProperties->bufferLength );
        __CPROVER_assume( pWillProperties->fieldSet >= 0 );
    }

    /* Before calling MQTT_SerializeConnect() it is up to the application to make
     * sure that the information in MQTTConnectInfo_t and MQTTPublishInfo_t can
     * fit into the MQTTFixedBuffer_t. It is a violation of the API to call
     * MQTT_SerializeConnect() without first calling MQTT_GetConnectPacketSize(). */
    if( pConnectInfo != NULL )
    {
        /* The output parameter pPacketSize of the function MQTT_GetConnectPacketSize()
         * must not be NULL. packetSize returned is not used in this proof, but
         * is used normally by the application to verify the size of its
         * MQTTFixedBuffer_t. MQTT_SerializeConnect() will use the remainingLength
         * to recalculate the packetSize. */
        status = MQTT_GetConnectPacketSize( pConnectInfo,
                                            pWillInfo,
                                            pConnectProperties,
                                            pWillProperties,
                                            &remainingLength,
                                            &packetSize );
    }

    if( status == MQTTSuccess )
    {
        /* For coverage, it is expected that a NULL pConnectInfo will reach this
         * function. */
        MQTT_SerializeConnect( pConnectInfo, pWillInfo, pConnectProperties, pWillProperties, remainingLength, pFixedBuffer );
    }

    // MQTTConnectInfo_t connectInfo = {0};
    // MQTTPublishInfo_t willInfo = {0};
    // size_t remainingLength;
    // size_t packetSize;
    // MQTTStatus_t status = MQTTSuccess;
    // MQTTFixedBuffer_t fixedBuffer = {0};
    // uint8_t buffer[100];
    // MQTTPropBuilder_t pConnectProperties={0};
    // MQTTPropBuilder_t pWillProperties={0};
    // uint8_t willpayload[3]={'a', 'b', 'c'};
    // uint8_t connectProps[3]={'a', 'b', 'c'};
    // uint8_t willProps[3]={'a', 'b', 'c'};

    // connectInfo.cleanSession = true;
    // connectInfo.pClientIdentifier = "sample_client_id";
    // connectInfo.clientIdentifierLength = 7U;
    // connectInfo.keepAliveSeconds = 60;
    // connectInfo.pUserName = "username";
    // connectInfo.userNameLength = 8U;
    // connectInfo.pPassword = "password";
    // connectInfo.passwordLength = 8U;

    // willInfo.pTopicName="my_topic";
    // willInfo.topicNameLength=7U;
    // willInfo.pPayload=(const void *) willpayload;
    // willInfo.payloadLength=3;

    // fixedBuffer.pBuffer = buffer;
    // fixedBuffer.size = 100U;

    // pConnectProperties.pBuffer=connectProps;
    // pConnectProperties.bufferLength=3;
    // pConnectProperties.currentIndex=3;

    // pWillProperties.pBuffer=willProps;
    // pWillProperties.bufferLength=3;
    // pWillProperties.currentIndex=3;

    // /* The output parameter pPacketSize of the function MQTT_GetConnectPacketSize()
    // * must not be NULL. packetSize returned is not used in this proof, but
    // * is used normally by the application to verify the size of its
    // * MQTTFixedBuffer_t. MQTT_SerializeConnect() will use the remainingLength
    // * to recalculate the packetSize. */
    // status = MQTT_GetConnectPacketSize( &connectInfo,
    //                                     &willInfo,
    //                                     &pConnectProperties,
    //                                     &pWillProperties,
    //                                     &remainingLength,
    //                                     &packetSize );
    
    // remainingLength+=0;
    // packetSize+=0;

    // if( status == MQTTSuccess )
    // {
    //     /* For coverage, it is expected that a NULL pConnectInfo will reach this
    //      * function. */
    //     MQTT_SerializeConnect( &connectInfo, &willInfo, &pConnectProperties, &pWillProperties, remainingLength, &fixedBuffer );
    // }
    // __CPROVER_assert(0, "debug");
}
