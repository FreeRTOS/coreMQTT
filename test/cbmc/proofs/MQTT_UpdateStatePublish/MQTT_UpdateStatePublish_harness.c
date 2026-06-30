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
 * @file MQTT_UpdateStatePublish_harness.c
 * @brief Implements the proof harness for MQTT_UpdateStatePublish function.
 *
 * @note The outgoing and incoming publish record arrays are wired up with a
 * non-zero max count, modelling a context after MQTT_InitStatefulQoS(). This is
 * the initialized case. (Were the arrays left NULL / max count 0, the QoS1/QoS2
 * receive path would reach addRecord(NULL, 0, ...) and trigger the same genuine
 * out-of-bounds read demonstrated by the MQTT_ReserveState proof, finding
 * state01_01.)
 */
#include <stdlib.h>
#include "core_mqtt.h"
#include "core_mqtt_state.h"
#include "mqtt_cbmc_state.h"

/* Number of publish records modelled per direction. MQTT_STATE_ARRAY_MAX_COUNT
 * is fixed at 10 by the proof config header and cannot be lowered with -D, so
 * the bound is set here and matched by the UNWINDSET in the Makefile. */
#ifndef RECORD_COUNT
    #define RECORD_COUNT    3U
#endif

void harness()
{
    MQTTContext_t * pContext;
    uint16_t packetId;
    MQTTStateOperation_t opType;
    MQTTQoS_t qos;
    MQTTPublishState_t * pNewState;

    pContext = allocateMqttContext( NULL );
    __CPROVER_assume( isValidMqttContext( pContext ) );
    __CPROVER_assume( pContext != NULL );

    /* Wire up initialized record arrays. The send path searches the outgoing
    * records (findInRecord) and updates them; the receive path adds to the
    * incoming records (addRecord). Record contents are non-deterministic. */
    pContext->outgoingPublishRecords = malloc( RECORD_COUNT * sizeof( MQTTPubAckInfo_t ) );
    __CPROVER_assume( pContext->outgoingPublishRecords != NULL );
    pContext->outgoingPublishRecordMaxCount = RECORD_COUNT;

    pContext->incomingPublishRecords = malloc( RECORD_COUNT * sizeof( MQTTPubAckInfo_t ) );
    __CPROVER_assume( pContext->incomingPublishRecords != NULL );
    pContext->incomingPublishRecordMaxCount = RECORD_COUNT;

    /* malloc may return NULL to cover the NULL-parameter check on pNewState. */
    pNewState = malloc( sizeof( MQTTPublishState_t ) );

    ( void ) MQTT_UpdateStatePublish( pContext, packetId, opType, qos, pNewState );
}
