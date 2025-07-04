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

MQTTStatus_t updatePointer( size_t * pPropertyLength )
{
    MQTTStatus_t status = MQTTSuccess;
    size_t length = *pPropertyLength;

    size_t decrement = ( length < MAX_LENGTH_OF_SINGLE_PROPERTY ) ? length : MAX_LENGTH_OF_SINGLE_PROPERTY;

    *pPropertyLength -= decrement;

    return status;
}

/*-----------------------------------------------------------*/

MQTTStatus_t __CPROVER_file_local_core_mqtt_serializer_c_decodeAndDiscard( size_t * pPropertyLength,
                                                                           uint8_t ** pIndex )
{
    return updatePointer( pPropertyLength );
}

/*-----------------------------------------------------------*/

MQTTStatus_t __CPROVER_file_local_core_mqtt_serializer_c_decodeAndDiscardutf_8( size_t * pPropertyLength,
                                                                                bool * pUsed,
                                                                                uint8_t ** pIndex )
{
    return updatePointer( pPropertyLength );
}

/*-----------------------------------------------------------*/

MQTTStatus_t __CPROVER_file_local_core_mqtt_serializer_c_decodeAndDiscard_uint8( size_t * pPropertyLength,
                                                                                 uint8_t ** pIndex )
{
    return updatePointer( pPropertyLength );
}

/*-----------------------------------------------------------*/
MQTTStatus_t __CPROVER_file_local_core_mqtt_serializer_c_decodeAndDiscard_uint32( size_t * pPropertyLength,
                                                                                  uint8_t ** pIndex )
{
    return updatePointer( pPropertyLength );
}

/*-----------------------------------------------------------*/

MQTTStatus_t __CPROVER_file_local_core_mqtt_serializer_c_decodeuint32_t( uint32_t * pProperty,
                                                                         size_t * pPropertyLength,
                                                                         bool * pUsed,
                                                                         uint8_t ** pIndex )
{
    return updatePointer( pPropertyLength );
}

/*-----------------------------------------------------------*/

MQTTStatus_t __CPROVER_file_local_core_mqtt_serializer_c_decodeuint16_t( uint16_t * pProperty,
                                                                         size_t * pPropertyLength,
                                                                         bool * pUsed,
                                                                         uint8_t ** pIndex )
{
    return updatePointer( pPropertyLength );
}

/*-----------------------------------------------------------*/

MQTTStatus_t __CPROVER_file_local_core_mqtt_serializer_c_decodeuint8_t( uint8_t * pProperty,
                                                                        size_t * pPropertyLength,
                                                                        bool * pUsed,
                                                                        uint8_t ** pIndex )
{
    return updatePointer( pPropertyLength );
}

/*-----------------------------------------------------------*/

MQTTStatus_t __CPROVER_file_local_core_mqtt_serializer_c_decodeutf_8( const char ** pProperty,
                                                                      uint16_t * pLength,
                                                                      size_t * pPropertyLength,
                                                                      bool * pUsed,
                                                                      uint8_t ** pIndex )
{
    return updatePointer( pPropertyLength );
}

/*-----------------------------------------------------------*/

MQTTStatus_t __CPROVER_file_local_core_mqtt_serializer_c_decodeBinaryData( const void ** pProperty,
                                                                           uint16_t * pLength,
                                                                           size_t * pPropertyLength,
                                                                           uint8_t ** pIndex )
{
    return updatePointer( pPropertyLength );
}
