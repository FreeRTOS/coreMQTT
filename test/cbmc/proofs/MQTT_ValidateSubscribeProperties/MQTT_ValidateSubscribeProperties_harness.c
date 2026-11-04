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
 * @file MQTT_ValidateSubscribeProperties_harness.c
 * @brief Implements the proof harness for MQTT_ValidateSubscribeProperties function.
 *
 * Note: MQTT_ValidateSubscribeProperties contains a known functional defect (a
 * subscription-id pointer is not advanced after decodeVariableLength, which can
 * cause a misparse). This is NOT a memory-safety defect: the length budget
 * (propertyLength) shrinks in lockstep so all buffer reads stay in bounds. This
 * harness proves memory safety only and is expected to reach
 * VERIFICATION SUCCESSFUL.
 */

#include "core_mqtt.h"
#include "core_mqtt_serializer.h"
#include "mqtt_cbmc_state.h"

void harness()
{
    MQTTPropBuilder_t * propBuffer;
    bool isSubscriptionIdAvailable;

    propBuffer = allocateMqttPropBuilder( NULL );
    __CPROVER_assume( isValidMqttPropBuilder( propBuffer ) );

    MQTT_ValidateSubscribeProperties( isSubscriptionIdAvailable, propBuffer );
}
