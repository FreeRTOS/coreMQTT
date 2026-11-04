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
 * @file MQTT_ReserveState_harness.c
 * @brief Implements the proof harness for MQTT_ReserveState function.
 *
 * @note This proof is EXPECTED TO FAIL. It exposes finding `state01_01`: a
 * freshly initialized MQTT context (after MQTT_Init but before
 * MQTT_InitStatefulQoS) has outgoingPublishRecords == NULL and
 * outgoingPublishRecordMaxCount == 0. MQTT_ReserveState only validates that
 * pMqttContext != NULL and the packet ID, NOT the outgoing record array. For a
 * QoS1/QoS2 reserve it therefore calls addRecord(NULL, 0, ...). The very first
 * statement of addRecord is `records[ recordCount - 1U ]`; with recordCount == 0
 * (a size_t) this underflows to records[SIZE_MAX], an out-of-bounds read on a
 * NULL array. The failure is genuine and is intentionally left unmasked.
 */
#include "core_mqtt.h"
#include "core_mqtt_state.h"
#include "mqtt_cbmc_state.h"

void harness()
{
    MQTTContext_t * pContext;
    uint16_t packetId;
    MQTTQoS_t qos;

    pContext = allocateMqttContext( NULL );
    __CPROVER_assume( isValidMqttContext( pContext ) );

    /* The context is intentionally left in its post-MQTT_Init state: the
     * outgoing publish record array is NULL and its max count is 0. The packet
     * ID and QoS are fully non-deterministic so that the QoS1/QoS2 path that
     * reaches addRecord(NULL, 0, ...) is explored. This surfaces the genuine
     * out-of-bounds read (finding state01_01). */
    ( void ) MQTT_ReserveState( pContext, packetId, qos );
}
