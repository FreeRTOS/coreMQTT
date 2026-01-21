/*
 * coreMQTT <DEVELOPMENT BRANCH>
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
 * @file event_callback_stub.c
 * @brief A stub for the event callback.
 */

#include <stdint.h>
#include "core_mqtt.h"

#ifndef MAX_UTF_8_STRING_LENGTH
    #define MAX_UTF_8_STRING_LENGTH    ( 10U )
#endif

/* Here we assume all the properties in the packet are user properties all of the same
 * length. Each user property includes two UTF-8 strings and
 * their corresponding 2 byte lengths.
 */
#define MAX_LENGTH_OF_SINGLE_PROPERTY    ( 2 * MAX_UTF_8_STRING_LENGTH + 4 )

static MQTTStatus_t updatePointer( uint32_t * pPropertyLength,
                                   uint8_t ** pIndex )
{
    MQTTStatus_t status = MQTTSuccess;
    uint32_t length = *pPropertyLength;

    uint32_t decrement = ( length < MAX_LENGTH_OF_SINGLE_PROPERTY ) ? length : MAX_LENGTH_OF_SINGLE_PROPERTY;

    *pPropertyLength -= decrement;
    *pIndex += decrement;

    return status;
}

/*-----------------------------------------------------------*/

MQTTStatus_t decodeUserProp( const char ** pPropertyKey,
                             uint16_t * pPropertyKeyLen,
                             const char ** pPropertyValue,
                             uint16_t * pPropertyValueLen,
                             uint32_t * pPropertyLength,
                             uint8_t ** pIndex )
{
    if( nondet_bool() )
    {
        return updatePointer( pPropertyLength, pIndex );
    }
    else
    {
        return MQTTBadResponse;
    }
}

/*-----------------------------------------------------------*/

MQTTStatus_t decodeUtf8( const char ** pProperty,
                         uint16_t * pLength,
                         uint32_t * pPropertyLength,
                         bool * pUsed,
                         uint8_t ** pIndex )
{
    if( nondet_bool() )
    {
        return updatePointer( pPropertyLength, pIndex );
    }
    else
    {
        return MQTTBadResponse;
    }
}

/*-----------------------------------------------------------*/

MQTTStatus_t decodeUint16t( uint16_t * pProperty,
                            uint32_t * pPropertyLength,
                            bool * pUsed,
                            uint8_t ** pIndex )
{
    if( nondet_bool() )
    {
        return updatePointer( pPropertyLength, pIndex );
    }
    else
    {
        return MQTTBadResponse;
    }
}

/*-----------------------------------------------------------*/

MQTTStatus_t decodeUint32t( uint32_t * pProperty,
                            uint32_t * pPropertyLength,
                            bool * pUsed,
                            uint8_t ** pIndex )
{
    if( nondet_bool() )
    {
        return updatePointer( pPropertyLength, pIndex );
    }
    else
    {
        return MQTTBadResponse;
    }
}

/*-----------------------------------------------------------*/

MQTTStatus_t decodeUint8t( uint8_t * pProperty,
                           uint32_t * pPropertyLength,
                           bool * pUsed,
                           uint8_t ** pIndex )
{
    if( nondet_bool() )
    {
        return updatePointer( pPropertyLength, pIndex );
    }
    else
    {
        return MQTTBadResponse;
    }
}
