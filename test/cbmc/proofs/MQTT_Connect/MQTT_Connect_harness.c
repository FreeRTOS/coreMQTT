/*
 * coreMQTT v2.0.0
 * Copyright (C) 2020 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
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
 * @file MQTT_Connect_harness.c
 * @brief Implements the proof harness for MQTT_Connect function.
 */
#include "core_mqtt.h"
#include "mqtt_cbmc_state.h"

void harness()
{
    MQTTContext_t * pContext;
    MQTTConnectInfo_t * pConnectInfo;
    MQTTPublishInfo_t * pWillInfo;
    uint32_t timeoutMs;
    size_t totalMessageLength = 0U;
    bool * pSessionPresent;

    pContext = allocateMqttContext( NULL );
    __CPROVER_assume( isValidMqttContext( pContext ) );
    __CPROVER_assume( pContext != NULL );
    __CPROVER_assume( pContext->networkBuffer.pBuffer != NULL );

    pConnectInfo = allocateMqttConnectInfo( NULL );
    __CPROVER_assume( isValidMqttConnectInfo( pConnectInfo ) );

    if( pConnectInfo != NULL )
    {
        /* 128^4 is the length imposed by the MQTT spec. */
        __CPROVER_assume( pConnectInfo->passwordLength < 268435456 );
        __CPROVER_assume( pConnectInfo->userNameLength < 268435456 );
        __CPROVER_assume( pConnectInfo->clientIdentifierLength < 268435456 );

        totalMessageLength += pConnectInfo->passwordLength;
        totalMessageLength += pConnectInfo->userNameLength;
        totalMessageLength += pConnectInfo->clientIdentifierLength;

        if( pConnectInfo->passwordLength == 0 )
        {
            pConnectInfo->pPassword = NULL;
        }
        else
        {
            __CPROVER_assume( pConnectInfo->pPassword != NULL );
        }

        if( pConnectInfo->userNameLength == 0 )
        {
            pConnectInfo->pUserName = NULL;
        }
        else
        {
            __CPROVER_assume( pConnectInfo->pUserName != NULL );
        }

        if( pConnectInfo->clientIdentifierLength == 0 )
        {
            pConnectInfo->pClientIdentifier = NULL;
        }
        else
        {
            __CPROVER_assume( pConnectInfo->pClientIdentifier != NULL );
        }
    }

    pWillInfo = allocateMqttPublishInfo( NULL );
    __CPROVER_assume( isValidMqttPublishInfo( pWillInfo ) );

    if( pWillInfo != NULL )
    {
        /* 128^4 is the length imposed by the MQTT spec. */
        __CPROVER_assume( pWillInfo->topicNameLength < 268435456 );
        __CPROVER_assume( pWillInfo->payloadLength < 268435456 );

        if( pWillInfo->topicNameLength == 0 )
        {
            pWillInfo->pTopicName = NULL;
        }
        else
        {
            __CPROVER_assume( pWillInfo->pTopicName != NULL );
        }

        if( pWillInfo->payloadLength == 0 )
        {
            pWillInfo->pPayload = NULL;
        }
        else
        {
            __CPROVER_assume( pWillInfo->pPayload != NULL );
        }

        totalMessageLength += pWillInfo->topicNameLength;
        totalMessageLength += pWillInfo->payloadLength;
    }

    /* 128^4 is the length imposed by the MQTT spec. */
    __CPROVER_assume( totalMessageLength <= 268435456 );

    pSessionPresent = malloc( sizeof( bool ) );

    /* The MQTT_RECEIVE_TIMEOUT is used here to control the number of loops
     * when receiving on the network. The default is used here because memory
     * safety can be proven in only a few iterations. */
    __CPROVER_assume( timeoutMs < MQTT_RECEIVE_TIMEOUT );

    MQTT_Connect( pContext, pConnectInfo, pWillInfo, timeoutMs, pSessionPresent );
}
