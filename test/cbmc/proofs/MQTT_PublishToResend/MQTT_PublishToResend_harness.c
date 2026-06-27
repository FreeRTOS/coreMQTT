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
 * @file MQTT_PublishToResend_harness.c
 * @brief Implements the proof harness for MQTT_PublishToResend function.
 */
#include <stdlib.h>
#include "core_mqtt.h"
#include "core_mqtt_state.h"
#include "mqtt_cbmc_state.h"

/* Number of outgoing publish records modelled. MQTT_STATE_ARRAY_MAX_COUNT is
 * fixed at 10 by the proof config header and cannot be lowered with -D, so the
 * bound is set here and matched by the UNWINDSET in the Makefile. */
#ifndef RECORD_COUNT
    #define RECORD_COUNT    3U
#endif

void harness()
{
    MQTTContext_t * pContext;
    MQTTStateCursor_t * pCursor;
    size_t i;

    pContext = allocateMqttContext( NULL );
    __CPROVER_assume( isValidMqttContext( pContext ) );
    __CPROVER_assume( pContext != NULL );

    /* Wire up an initialized outgoing publish record array, modelling a context
     * after MQTT_InitStatefulQoS(). stateSelect() scans these records. Record
     * contents are non-deterministic. */
    pContext->outgoingPublishRecords = malloc( RECORD_COUNT * sizeof( MQTTPubAckInfo_t ) );
    __CPROVER_assume( pContext->outgoingPublishRecords != NULL );
    pContext->outgoingPublishRecordMaxCount = RECORD_COUNT;

    /* stateSelect() uses each record's publishState as a left-shift amount
     * (1U << publishState) to build a 16-bit state bitmap. The library only ever
     * stores valid MQTTPublishState_t values in the records, so constrain the
     * non-deterministic record contents to the valid enum range. Without this,
     * an out-of-range shift amount would be flagged (an unreachable scenario in
     * normal operation). */
    for( i = 0; i < RECORD_COUNT; i++ )
    {
        __CPROVER_assume( pContext->outgoingPublishRecords[ i ].publishState >= MQTTStateNull );
        __CPROVER_assume( pContext->outgoingPublishRecords[ i ].publishState <= MQTTPublishDone );
    }

    /* malloc may return NULL to cover the NULL-parameter check on pCursor. The
     * cursor value is non-deterministic to cover every starting position. */
    pCursor = malloc( sizeof( MQTTStateCursor_t ) );

    ( void ) MQTT_PublishToResend( pContext, pCursor );
}
