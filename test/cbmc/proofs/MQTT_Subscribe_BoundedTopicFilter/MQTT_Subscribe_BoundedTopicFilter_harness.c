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
 * @file MQTT_Subscribe_BoundedTopicFilter_harness.c
 * @brief Proof harness for MQTT_Subscribe when the broker has reported that
 * wildcard subscriptions are not available. In that branch coreMQTT scans the
 * caller-supplied topic filter for the wildcard characters '#' and '+'. The
 * scan must be bounded by MQTTSubscribeInfo_t::topicFilterLength so it never
 * reads past the buffer that the caller allocated.
 *
 * allocateMqttSubscriptionList() backs each pTopicFilter with a malloc of
 * exactly topicFilterLength bytes (no NUL terminator). Any read past that
 * boundary is an out-of-bounds access CBMC will catch.
 */

#include "core_mqtt.h"
#include "mqtt_cbmc_state.h"
#include "core_mqtt_config_defaults.h"

/**
 * @brief Implement a get time function to return timeout after certain
 * iterations have been made in the code. This ensures that we do not hit
 * unwinding error in CBMC.
 */
static uint32_t ulGetTimeFunction( void )
{
    static uint32_t systemTime = 0;

    if( systemTime >= MAX_NETWORK_SEND_TRIES )
    {
        systemTime = systemTime + MQTT_SEND_TIMEOUT_MS + 1;
    }
    else
    {
        systemTime = systemTime + 1;
    }

    return systemTime;
}

MQTTStatus_t MQTT_ValidateSubscribeProperties( bool isSubscriptionIdAvailable,
                                               const MQTTPropBuilder_t * propBuilder )
{
    MQTTStatus_t status;

    return status;
}

void harness()
{
    MQTTContext_t * pContext;
    MQTTSubscribeInfo_t * pSubscriptionList;
    MQTTPropBuilder_t * propBuffer;
    uint16_t packetId;

    pContext = allocateMqttContext( NULL );
    __CPROVER_assume( isValidMqttContext( pContext ) );

    if( pContext != NULL )
    {
        pContext->getTime = ulGetTimeFunction;

        /* Force the validation path that scans the topic filter for the
         * wildcard characters. */
        pContext->connectionProperties.isWildcardAvailable = 0U;
    }

    pSubscriptionList = allocateMqttSubscriptionList( NULL, 1U );
    __CPROVER_assume( isValidMqttSubscriptionList( pSubscriptionList, 1U ) );

    propBuffer = allocateMqttPropBuilder( NULL );
    __CPROVER_assume( isValidMqttPropBuilder( propBuffer ) );

    MQTT_Subscribe( pContext, pSubscriptionList, 1U, packetId, propBuffer );
}
