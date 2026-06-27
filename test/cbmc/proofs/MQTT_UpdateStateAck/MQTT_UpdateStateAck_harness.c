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
 * @file MQTT_UpdateStateAck_harness.c
 * @brief Implements the proof harness for MQTT_UpdateStateAck function.
 */
#include <stdlib.h>
#include "core_mqtt.h"
#include "core_mqtt_state.h"
#include "mqtt_cbmc_state.h"

/* Number of publish records modelled per direction. See the note in the
 * Makefile: MQTT_STATE_ARRAY_MAX_COUNT is fixed at 10 by the proof config
 * header and cannot be lowered with -D, so the bound is set here and matched by
 * the UNWINDSET. */
#ifndef RECORD_COUNT
    #define RECORD_COUNT    3U
#endif

void harness()
{
    MQTTContext_t * pContext;
    uint16_t packetId;
    MQTTPubAckType_t packetType;
    MQTTStateOperation_t opType;
    MQTTPublishState_t * pNewState;

    pContext = allocateMqttContext( NULL );
    __CPROVER_assume( isValidMqttContext( pContext ) );
    __CPROVER_assume( pContext != NULL );

    /* Wire up initialized outgoing and incoming publish record arrays, modelling
     * a context after MQTT_InitStatefulQoS(). MQTT_UpdateStateAck selects one of
     * the two arrays depending on the ack direction, and may reach addRecord()
     * (for the PUBREC -> PUBREL transition). Record contents are
     * non-deterministic. */
    pContext->outgoingPublishRecords = malloc( RECORD_COUNT * sizeof( MQTTPubAckInfo_t ) );
    __CPROVER_assume( pContext->outgoingPublishRecords != NULL );
    pContext->outgoingPublishRecordMaxCount = RECORD_COUNT;

    pContext->incomingPublishRecords = malloc( RECORD_COUNT * sizeof( MQTTPubAckInfo_t ) );
    __CPROVER_assume( pContext->incomingPublishRecords != NULL );
    pContext->incomingPublishRecordMaxCount = RECORD_COUNT;

    /* malloc may return NULL to cover the NULL-parameter check on pNewState. */
    pNewState = malloc( sizeof( MQTTPublishState_t ) );

    ( void ) MQTT_UpdateStateAck( pContext, packetId, packetType, opType, pNewState );
}
