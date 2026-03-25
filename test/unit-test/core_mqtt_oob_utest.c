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
 * @file core_mqtt_oob_utest.c
 * @brief Unit tests for functions in core_mqtt_serializer.h.
 */
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include "unity.h"


/* Include paths for public enums, structures, and macros. */
#include "core_mqtt_serializer.h"

#include "../core_mqtt_serializer.c"

void test_MQTTV5_suback_outOfBoundAccess( void )
{
    MQTTStatus_t status;
    uint8_t packetBuffer[ 33 ] =
    {
        0x90,             /* Fixed header: SUBACK type (0x90) */
        0x15,             /* Remaining Length = 21 bytes */
        0x00, 0x01,       /* Packet Identifier = 1 */
        0x1C,             /* Property Length = 28 bytes -> INVALID - properties can only be 19 (21 - 2) bytes long at max. */
        0x1F,             /* Property ID = 0x1F (Reason String) */
        0x00, 0x03,       /* UTF-8 string length = 3 */
        0x61, 0x62, 0x63, /* The string "abc" */
        0x26,             /* Property ID = 0x26 (User Property) */
        0x00, 0x03,       /* UTF-8 string length = 3 */
        0x61, 0x62, 0x63, /* The string "abc" */
        0x00, 0x03,       /* UTF-8 string length = 3 */
        0x61, 0x62, 0x63, /* The string "abc" */
        /* FOLLOWING is the payload - masquerading as a property. */
        0x26,             /* Property ID = 0x26 (User Property) */

        /* ======= Packet end all the following bytes will be out of bound reads ===== */
        0x00, 0x03,       /* UTF-8 string length = 3 */
        0x61, 0x62, 0x63, /* The string "abc" */
        0x00, 0x03,       /* UTF-8 string length = 3 */
        0x61, 0x62, 0x63, /* The string "abc" */
    };

    MQTTPropBuilder_t propBuffer = { 0 };
    size_t propertyLength = 0U;

    status = deserializeSubUnsubAckProperties( &propBuffer,
                                               &packetBuffer[ 4 ], /* Beginning of prop length. */
                                               &propertyLength,
                                               packetBuffer[ 1 ] /* The remaining length. */ );
    /* Must fail. */
    TEST_ASSERT_EQUAL( MQTTBadResponse, status );
}
