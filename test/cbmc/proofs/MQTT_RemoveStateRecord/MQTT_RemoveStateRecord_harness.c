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
 * @file MQTT_RemoveStateRecord_harness.c
 * @brief Implements the proof harness for MQTT_RemoveStateRecord function.
 */
#include <stdlib.h>
#include "core_mqtt.h"
#include "core_mqtt_state.h"
#include "mqtt_cbmc_state.h"

/* The number of outgoing publish records modelled. The library normally
 * receives this count from MQTT_InitStatefulQoS(). A small bound keeps the
 * record loops tractable. (Note: MQTT_STATE_ARRAY_MAX_COUNT is defined
 * unconditionally as 10 in test/cbmc/include/core_mqtt_config.h, so it cannot
 * be lowered with a -D flag; the bound is fixed here instead and matched by the
 * UNWINDSET in the Makefile.) */
#ifndef RECORD_COUNT
    #define RECORD_COUNT    3U
#endif

void harness()
{
    MQTTContext_t * pContext;
    uint16_t packetId;

    pContext = allocateMqttContext( NULL );
    __CPROVER_assume( isValidMqttContext( pContext ) );
    __CPROVER_assume( pContext != NULL );

    /* Wire up an initialized outgoing publish record array, modelling a context
     * after MQTT_InitStatefulQoS(). malloc may return NULL to cover the
     * NULL-array branch in MQTT_RemoveStateRecord. The record contents are left
     * non-deterministic. */
    pContext->outgoingPublishRecords = malloc( RECORD_COUNT * sizeof( MQTTPubAckInfo_t ) );
    pContext->outgoingPublishRecordMaxCount = RECORD_COUNT;

    /* findInRecord() asserts that the packet ID is not the invalid sentinel.
     * MQTT_RemoveStateRecord does not itself guard the packet ID before calling
     * findInRecord, so model a meaningful (non-zero) packet ID. */
    __CPROVER_assume( packetId != MQTT_PACKET_ID_INVALID );

    ( void ) MQTT_RemoveStateRecord( pContext, packetId );
}
