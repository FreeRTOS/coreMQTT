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
 * @file MQTT_InitStatefulQoS_harness.c
 * @brief Implements the proof harness for MQTT_InitStatefulQoS function.
 */
#include "core_mqtt.h"
#include "mqtt_cbmc_state.h"

/* A small bound on the number of QoS publish records. MQTT_InitStatefulQoS()
 * only stores the array pointers and counts (it does not iterate over them), so
 * a small bound keeps the malloc sizes tractable while still covering the
 * argument-consistency checks. */
#ifndef RECORD_COUNT_MAX
    #define RECORD_COUNT_MAX    2U
#endif

/* A small bound on the ACK properties buffer length. */
#ifndef ACK_PROPS_BUF_MAX
    #define ACK_PROPS_BUF_MAX    10U
#endif

void harness()
{
    MQTTContext_t * pContext;
    MQTTPubAckInfo_t * pOutgoingPublishRecords;
    size_t outgoingPublishCount;
    MQTTPubAckInfo_t * pIncomingPublishRecords;
    size_t incomingPublishCount;
    uint8_t * pAckPropsBuf;
    size_t ackPropsBufLength;

    /* allocateMqttContext() calls MQTT_Init() internally, which sets a non-NULL
     * appCallback (EventCallbackStub). The function requires appCallback != NULL
     * (i.e. MQTT_Init was called) before it succeeds. It may also return NULL to
     * cover the NULL-parameter check. */
    pContext = allocateMqttContext( NULL );
    __CPROVER_assume( isValidMqttContext( pContext ) );

    /* Bound the record counts and allocate matching arrays. malloc() may return
     * NULL; the function checks that (count == 0) == (records == NULL). The
     * record contents are left non-deterministic - the function only stores the
     * array pointer, it never dereferences the records. */
    __CPROVER_assume( outgoingPublishCount <= RECORD_COUNT_MAX );
    __CPROVER_assume( incomingPublishCount <= RECORD_COUNT_MAX );
    pOutgoingPublishRecords = malloc( outgoingPublishCount * sizeof( MQTTPubAckInfo_t ) );
    pIncomingPublishRecords = malloc( incomingPublishCount * sizeof( MQTTPubAckInfo_t ) );

    /* Bound the ACK properties buffer length. malloc() may return NULL to cover
     * the branch where the internal property buffer is left unset. */
    __CPROVER_assume( ackPropsBufLength <= ACK_PROPS_BUF_MAX );
    pAckPropsBuf = malloc( ackPropsBufLength );

    MQTT_InitStatefulQoS( pContext,
                          pOutgoingPublishRecords,
                          outgoingPublishCount,
                          pIncomingPublishRecords,
                          incomingPublishCount,
                          pAckPropsBuf,
                          ackPropsBufLength );
}
