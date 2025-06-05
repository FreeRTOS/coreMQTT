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
 * @file MQTT_DeserializePublish_harness.c
 * @brief Implements the proof harness for MQTT_DeserializePublish function.
 */

#include "core_mqtt.h"
#include "mqtt_cbmc_state.h"

#ifndef REMAINING_LENGTH_MAX
    #define REMAINING_LENGTH_MAX    CBMC_MAX_OBJECT_SIZE
#endif

void harness()
{
    uint8_t ** buffer;
    size_t * length;
    bool * used;
    uint32_t * property_uint32_t;
    uint32_t * property_uint16_t;
    uint32_t * property_uint8_t;
    const char ** property_utf_8;
    const void ** property_binary;
    uint16_t * property_length;

    length = malloc( sizeof( size_t ) );

    __CPROVER_assume( length != NULL );
    __CPROVER_assume( *length >= 0 );

    /* This buffer is used to store packet properties. The property length
     *  is a variable length integer and hence will have a max value of REMAINING_LENGTH_MAX */
    __CPROVER_assume( *length < REMAINING_LENGTH_MAX );

    buffer = malloc( sizeof( uint8_t * ) );

    __CPROVER_assume( buffer != NULL );

    *buffer = malloc( *length );

    __CPROVER_assume( *buffer != NULL );

    __CPROVER_file_local_core_mqtt_serializer_c_decodeAndDiscard( length, buffer );

    /*-----------------------------------------------------------*/

    length = malloc( sizeof( size_t ) );

    __CPROVER_assume( length != NULL );
    __CPROVER_assume( *length >= 0 );

    /* This buffer is used to store packet properties. The property length
     *  is a variable length integer and hence will have a max value of REMAINING_LENGTH_MAX */
    __CPROVER_assume( *length < REMAINING_LENGTH_MAX );

    used = malloc( sizeof( bool ) );

    __CPROVER_assume( used != NULL );

    buffer = malloc( sizeof( uint8_t * ) );

    __CPROVER_assume( buffer != NULL );

    *buffer = malloc( *length );

    __CPROVER_assume( *buffer != NULL );

    __CPROVER_file_local_core_mqtt_serializer_c_decodeAndDiscardutf_8( length, used, buffer );

    /*-----------------------------------------------------------*/

    length = malloc( sizeof( size_t ) );

    __CPROVER_assume( length != NULL );
    __CPROVER_assume( *length >= 0 );

    /* This buffer is used to store packet properties. The property length
     *  is a variable length integer and hence will have a max value of REMAINING_LENGTH_MAX */
    __CPROVER_assume( *length < REMAINING_LENGTH_MAX );

    used = malloc( sizeof( bool ) );

    __CPROVER_assume( used != NULL );

    buffer = malloc( sizeof( uint8_t * ) );

    __CPROVER_assume( buffer != NULL );

    *buffer = malloc( *length );

    __CPROVER_assume( *buffer != NULL );

    __CPROVER_file_local_core_mqtt_serializer_c_decodeAndDiscard_uint8( length, used, buffer );


    /*-----------------------------------------------------------*/

    length = malloc( sizeof( size_t ) );

    __CPROVER_assume( length != NULL );
    __CPROVER_assume( *length >= 0 );

    /* This buffer is used to store packet properties. The property length
     *  is a variable length integer and hence will have a max value of REMAINING_LENGTH_MAX */
    __CPROVER_assume( *length < REMAINING_LENGTH_MAX );

    used = malloc( sizeof( bool ) );

    __CPROVER_assume( used != NULL );

    buffer = malloc( sizeof( uint8_t * ) );

    __CPROVER_assume( buffer != NULL );

    *buffer = malloc( *length );

    __CPROVER_assume( *buffer != NULL );

    __CPROVER_file_local_core_mqtt_serializer_c_decodeAndDiscard_uint32( length, used, buffer );

    /*-----------------------------------------------------------*/

    property_uint32_t = malloc( sizeof( uint32_t ) );

    __CPROVER_assume( property_uint32_t != NULL );

    length = malloc( sizeof( size_t ) );

    __CPROVER_assume( length != NULL );
    __CPROVER_assume( *length >= 0 );

    /* This buffer is used to store packet properties. The property length
     *  is a variable length integer and hence will have a max value of REMAINING_LENGTH_MAX */
    __CPROVER_assume( *length < REMAINING_LENGTH_MAX );

    used = malloc( sizeof( bool ) );

    __CPROVER_assume( used != NULL );

    buffer = malloc( sizeof( uint8_t * ) );

    __CPROVER_assume( buffer != NULL );

    *buffer = malloc( *length );

    __CPROVER_assume( *buffer != NULL );

    __CPROVER_file_local_core_mqtt_serializer_c_decodeuint32_t( property_uint32_t, length, used, buffer );

    /*-----------------------------------------------------------*/

    property_uint16_t = malloc( sizeof( uint16_t ) );

    __CPROVER_assume( property_uint16_t != NULL );

    length = malloc( sizeof( size_t ) );

    __CPROVER_assume( length != NULL );
    __CPROVER_assume( *length >= 0 );

    /* This buffer is used to store packet properties. The property length
     *  is a variable length integer and hence will have a max value of REMAINING_LENGTH_MAX */
    __CPROVER_assume( *length < REMAINING_LENGTH_MAX );

    used = malloc( sizeof( bool ) );

    __CPROVER_assume( used != NULL );

    buffer = malloc( sizeof( uint8_t * ) );

    __CPROVER_assume( buffer != NULL );

    *buffer = malloc( *length );

    __CPROVER_assume( *buffer != NULL );

    __CPROVER_file_local_core_mqtt_serializer_c_decodeuint16_t( property_uint16_t, length, used, buffer );

    /*-----------------------------------------------------------*/

    property_uint8_t = malloc( sizeof( uint8_t ) );

    __CPROVER_assume( property_uint8_t != NULL );

    length = malloc( sizeof( size_t ) );

    __CPROVER_assume( length != NULL );
    __CPROVER_assume( *length >= 0 );

    /* This buffer is used to store packet properties. The property length
     *  is a variable length integer and hence will have a max value of REMAINING_LENGTH_MAX */
    __CPROVER_assume( *length < REMAINING_LENGTH_MAX );

    used = malloc( sizeof( bool ) );

    __CPROVER_assume( used != NULL );

    buffer = malloc( sizeof( uint8_t * ) );

    __CPROVER_assume( buffer != NULL );

    *buffer = malloc( *length );

    __CPROVER_assume( *buffer != NULL );

    __CPROVER_file_local_core_mqtt_serializer_c_decodeuint8_t( property_uint8_t, length, used, buffer );

    /*-----------------------------------------------------------*/

    property_utf_8 = malloc( sizeof( char * ) );

    __CPROVER_assume( property_utf_8 != NULL );

    property_length = malloc( sizeof( uint16_t ) );

    __CPROVER_assume( property_length != NULL );

    length = malloc( sizeof( size_t ) );

    __CPROVER_assume( length != NULL );
    __CPROVER_assume( *length >= 0 );

    /* This buffer is used to store packet properties. The property length
     *  is a variable length integer and hence will have a max value of REMAINING_LENGTH_MAX */
    __CPROVER_assume( *length < REMAINING_LENGTH_MAX );

    used = malloc( sizeof( bool ) );

    __CPROVER_assume( used != NULL );

    buffer = malloc( sizeof( uint8_t * ) );

    __CPROVER_assume( buffer != NULL );

    *buffer = malloc( *length );

    __CPROVER_assume( *buffer != NULL );

    __CPROVER_file_local_core_mqtt_serializer_c_decodeutf_8( property_utf_8, property_length, length, used, buffer );

    /*-----------------------------------------------------------*/

    property_binary = malloc( sizeof( char * ) );

    __CPROVER_assume( property_binary != NULL );

    property_length = malloc( sizeof( uint16_t ) );

    __CPROVER_assume( property_length != NULL );

    length = malloc( sizeof( size_t ) );

    __CPROVER_assume( length != NULL );
    __CPROVER_assume( *length >= 0 );

    /* This buffer is used to store packet properties. The property length
     *  is a variable length integer and hence will have a max value of REMAINING_LENGTH_MAX */
    __CPROVER_assume( *length < REMAINING_LENGTH_MAX );

    used = malloc( sizeof( bool ) );

    __CPROVER_assume( used != NULL );

    buffer = malloc( sizeof( uint8_t * ) );

    __CPROVER_assume( buffer != NULL );

    *buffer = malloc( *length );

    __CPROVER_assume( *buffer != NULL );

    __CPROVER_file_local_core_mqtt_serializer_c_decodeutf_8( property_binary, property_length, length, used, buffer );
}
