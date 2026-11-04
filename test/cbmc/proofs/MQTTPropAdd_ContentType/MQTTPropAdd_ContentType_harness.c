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
 * @file MQTTPropAdd_ContentType_harness.c
 * @brief Proof harness for MQTTPropAdd_ContentType, which exercises the write
 * path of the file-local addPropUtf8 helper. This proves that the bounds check
 * reserves space for the full encoding (1 byte property ID + 2 byte UTF-8
 * length prefix + the string itself) and never writes past the property
 * builder buffer.
 */

#include "core_mqtt.h"
#include "mqtt_cbmc_state.h"

/* Bound the property length to keep the proof tractable. The memory-safety
 * properties proven here are independent of the specific length, so this
 * bound only needs to be large enough to exercise the encoding paths. */
#ifndef MAX_CONTENT_TYPE_LENGTH
    #define MAX_CONTENT_TYPE_LENGTH    32U
#endif

void harness()
{
    MQTTPropBuilder_t * propBuffer;
    char * pContentType;
    size_t contentTypeLength;

    /* allocateMqttPropBuilder backs the builder with a malloc'd buffer of exactly
     * bufferLength bytes and a non-deterministic currentIndex in [0, bufferLength),
     * so any write past bufferLength is an out-of-bounds access CBMC will catch. */
    propBuffer = allocateMqttPropBuilder( NULL );
    __CPROVER_assume( isValidMqttPropBuilder( propBuffer ) );

    __CPROVER_assume( contentTypeLength <= MAX_CONTENT_TYPE_LENGTH );
    pContentType = malloc( contentTypeLength );

    ( void ) MQTTPropAdd_ContentType( propBuffer, pContentType, contentTypeLength, NULL );
}
