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
 * @file MQTTPropertyBuilder_Init_harness.c
 * @brief Implements the proof harness for MQTTPropertyBuilder_Init function.
 */

#include "core_mqtt.h"
#include "core_mqtt_serializer.h"
#include "mqtt_cbmc_state.h"

#ifndef PROP_BUFFER_LENGTH_MAX
    #define PROP_BUFFER_LENGTH_MAX    ( 10U )
#endif

void harness()
{
    MQTTPropBuilder_t * pPropertyBuilder;
    uint8_t * buffer;
    size_t length;

    /* The builder struct may be NULL to exercise the NULL-parameter check. */
    pPropertyBuilder = malloc( sizeof( MQTTPropBuilder_t ) );

    /* Bound the buffer to a small length. The function only stores the buffer
     * pointer and length; it never dereferences the buffer. buffer may be NULL
     * to exercise the NULL-parameter check. */
    __CPROVER_assume( length <= PROP_BUFFER_LENGTH_MAX );
    buffer = malloc( length );

    MQTTPropertyBuilder_Init( pPropertyBuilder, buffer, length );
}
